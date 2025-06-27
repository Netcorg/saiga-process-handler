#include <sstream>
#include "saiga_database_manager.h"
#include "spdlog/spdlog.h"

static int empty_callback(void *param, int argc, char **argv, char **col_name) {
  return 0;
}

static int fetch_list_callback(void *param, int argc, char **argv, char **col_name) {
  if (6 != argc) {
    spdlog::error("invalid output from the table, argument count is {}", argc);
    return -1;
  }
  
  std::vector<Saiga::Process> *process_list = (std::vector<Saiga::Process> *) param;
  Saiga::Process process;
  
  process.pid = std::stoi(argv[0]);
  process.hwnd = std::stoi(argv[1]);
  process.time_tag = std::stoi(argv[2]);
  process.name = argv[3];
  process.title = argv[4];
  process.state = (Saiga::ProcessState) std::stoi(argv[5]);

  process_list->push_back(process);
  
  return 0;
}

Saiga::DatabaseManager *Saiga::DatabaseManager::m_instance = nullptr;

Saiga::DatabaseManager::DatabaseManager(void) {

}

Saiga::DatabaseManager::~DatabaseManager(void) {

}

Saiga::DatabaseManager *Saiga::DatabaseManager::getInstance(void) {
  if (nullptr == m_instance) {
    m_instance = new DatabaseManager();
  }

  return m_instance;
}


bool Saiga::DatabaseManager::open(const std::string &db_file) {
  if (nullptr != m_database) {
    spdlog::error("could not open database that is already opened");
    return false;
  }

  if (db_file.empty()) {
    spdlog::error("could not open database, its name is empty");
    return false;
  }
  
  return SQLITE_OK == sqlite3_open_v2(db_file.c_str(), &m_database, SQLITE_OPEN_READWRITE, nullptr);
}

bool Saiga::DatabaseManager::close(void) {
  if (nullptr == m_database) {
    spdlog::error("could not close database that is not opened yet");
    return false;
  }

  return SQLITE_OK == sqlite3_close_v2(m_database);
}

bool Saiga::DatabaseManager::insert(const Saiga::Process &process) {
  if (nullptr == m_database) {
    spdlog::error("could not insert process into database that is not opened yet");
    return false;
  }

  char *err_msg = nullptr;
  std::stringstream query;

  query << "INSERT INTO process VALUES (\'" <<
    std::to_string(process.pid) << "\', \'" <<
    std::to_string(process.hwnd) << "\', \'" <<
    process.time_tag << "\', \'" <<
    process.name << "\', \'" <<
    process.title << "\', \'" <<
    (ProcessState::CREATED == process.state ? "1\');" : "2\');");
			    
  spdlog::debug("query string: {}", query.str());

  if (SQLITE_OK != sqlite3_exec(m_database, query.str().c_str(), empty_callback, nullptr, &err_msg)) {
    spdlog::error("insertion to database failed, {}", err_msg);
    return false;
  }

  return true;
}

bool Saiga::DatabaseManager::insert(const std::vector<Saiga::Process> &process_list) {
  bool is_inserted = true;
  
  for (auto process : process_list) {
    is_inserted = is_inserted & insert(process);
  }

  // if there is not process to be inserted, it means valid insertion
  // or if there is merely one insertion failed, it means invalid insertion
  return is_inserted;
}

bool Saiga::DatabaseManager::fetch(std::vector<Saiga::FilteredProcess> &process_list, const uint32_t start_time, const uint32_t end_time) {
  if (nullptr == m_database) {
    spdlog::error("could not fetch process into database that is not opened yet");
    return false;
  }

  if (start_time >= end_time) {
    spdlog::error("invalid range values start is {}, end is {}", start_time, end_time);
    return false;
  }  

  const char *query = R"(
    SELECT
        name,
        COUNT(*) AS session_count,
        SUM(duration) AS total_duration
    FROM (
        SELECT
            app1.name,
            (app2.end_time - app1.start_time) AS duration
        FROM
            (SELECT pid, hwnd, name, time_tag AS start_time
             FROM process
             WHERE state = 1 AND time_tag > ?) AS app1
        JOIN
            (SELECT pid, hwnd, name, time_tag AS end_time
             FROM process
             WHERE state = 2 AND time_tag < ?) AS app2
        ON app1.pid = app2.pid
           AND app1.hwnd = app2.hwnd
           AND app1.name = app2.name
    )
    GROUP BY name ORDER BY duration;)";

  sqlite3_stmt* stmt = nullptr;

  if (SQLITE_OK != sqlite3_prepare_v2(m_database, query, -1, &stmt, nullptr)) {
    spdlog::error("could not prepare query, error is {}", sqlite3_errmsg(m_database));
    return false;
  }
 
  char *err_msg = nullptr;

  sqlite3_bind_int(stmt, 1, start_time);
  sqlite3_bind_int(stmt, 2, end_time);

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    FilteredProcess process;
    
    process.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    process.session_count = sqlite3_column_int(stmt, 1);
    process.duration = sqlite3_column_int(stmt, 2);

    process_list.push_back(process);
  }
  
  for (auto process : process_list) {
    spdlog::debug("process: {}, {}, {}", process.name, process.session_count, process.duration);
  }

  return true;
}

void Saiga::DatabaseManager::toJSON(const std::vector<Saiga::FilteredProcess> &process_list, std::string &json_text) {
  rapidjson::Value object_list(rapidjson::kArrayType);
  rapidjson::Document document;

  document.SetObject();
  json_text.clear();
  
  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
  
  for (auto process : process_list) {
    rapidjson::Value object(rapidjson::kObjectType);

    object.AddMember("name", rapidjson::Value().SetString(process.name.c_str(), allocator), allocator);
    object.AddMember("count", process.session_count, allocator);
    object.AddMember("duration", process.duration, allocator);

    object_list.PushBack(object, allocator);
  }

  document.AddMember("process_list", object_list, allocator);

  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

  document.Accept(writer);
  json_text = buffer.GetString();

  return;
}
