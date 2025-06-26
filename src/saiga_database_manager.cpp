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
  process.ppid = std::stoi(argv[1]);
  process.app_name = argv[2];
  process.title = argv[3];
  process.time = argv[4];
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
    std::to_string(process.ppid) << "\', \'" <<
    process.app_name << "\', \'" <<
    process.title << "\', \'" <<
    process.time << "\', \'" <<
    (ProcessState::CREATED == process.state ? "1\');" : "2\');");
			    
  spdlog::debug("query string: {}", query.str());

  if (SQLITE_OK != sqlite3_exec(m_database, query.str().c_str(), empty_callback, nullptr, &err_msg)) {
    spdlog::error("insertion to database failed, {}", err_msg);
    return false;
  }

  return true;
}

bool Saiga::DatabaseManager::fetch(std::vector<Saiga::Process> &process_list) {
  if (nullptr == m_database) {
    spdlog::error("could not fetch process into database that is not opened yet");
    return false;
  }

  char *err_msg = nullptr;
  std::string query = "select * from process";
  
  spdlog::debug("query string: {}", query);

  if (SQLITE_OK != sqlite3_exec(m_database, query.c_str(), fetch_list_callback, &process_list, &err_msg)) {
    spdlog::error("insertion to database failed, {}", err_msg);
    return false;
  }

  for (auto process : process_list) {
    spdlog::debug("process: {}", process.toString());
  }

  return true;
}
