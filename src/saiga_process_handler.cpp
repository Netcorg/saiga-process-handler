#include "saiga_process_handler.h"
#include "spdlog/spdlog.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

Saiga::ProcessHandler::ProcessHandler(const std::string &name)
  : EAR::Schedule::Task(name)
{

}

Saiga::ProcessHandler::~ProcessHandler() {
  if (nullptr != m_database_manager) {
    m_database_manager->close();
  }

  m_endpoint.terminate();
}

bool Saiga::ProcessHandler::configure(const std::string &config_file) {
  m_preferences.db_file = "db/saiga.db";

  /// @todo set endpoint configuration from config file, for now they're defauilt values
  
  return true;
}

bool Saiga::ProcessHandler::initialize(void) {
  m_database_manager = Saiga::DatabaseManager::getInstance();

  if (!m_database_manager->open(m_preferences.db_file)) {
    spdlog::error("could initialize database");
    return false;
  }

  if (!m_endpoint.initialize(m_preferences.endpoint_config)) {
    spdlog::error("could initialize endpoint");
    return false;
  }

  return true;
}


void Saiga::ProcessHandler::cycle(void)  {
  spdlog::debug("process handler cycle called");

  std::vector<Process> process_list;
  std::vector<FilteredProcess> filtered_process_list;
  std::string json_text;

  // handle process and put into database
  handleProcess(process_list);
  /// @todo insert into database if the process is created or killed
  m_database_manager->insert(process_list);
  // fetch database filtered, and convert to JSON
  m_database_manager->fetch(filtered_process_list, 0, 999999999);
  toJSON(filtered_process_list, json_text);
  // send to server
  m_endpoint.transmit(json_text);

  spdlog::debug("json text: {}", json_text);
  
  return;
} 

void Saiga::ProcessHandler::toJSON(const std::vector<Saiga::FilteredProcess> &process_list, std::string &json_text) {
  rapidjson::Value object_list(rapidjson::kArrayType);
  rapidjson::Document document;

  document.SetObject();
  json_text.clear();
  
  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
  
  for (auto process : process_list) {
    rapidjson::Value object(rapidjson::kObjectType);

    object.AddMember("name", rapidjson::Value().SetString(process.app_name.c_str(), allocator), allocator);
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

void Saiga::ProcessHandler::handleProcess(std::vector<Process> &process_list) {
  return;
}
