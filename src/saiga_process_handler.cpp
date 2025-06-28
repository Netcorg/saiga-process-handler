#include "saiga_process_handler.h"
#include "spdlog/spdlog.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include <fstream>
#include <sstream>
#include <chrono>
#include <windows.h>
#include <cstdlib>
#include <codecvt>
#include <dwmapi.h>
#include <shlwapi.h>
#include <psapi.h>

#define _AMD64_
#include <winuser.h>

/// maximum length for a process path
#define MAX_PROCESS_PATH_LEN (512)

using namespace std::chrono;

/// unicode converter for wide characters
static std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

/// function that checks whether window is visible on user screen
/// @return TRUE if the window is visible, otherwise false
static inline BOOL isWindowVisibleOnScreen(HWND hwnd) {
  BOOL isCloaked = FALSE;
  return IsWindowVisible(hwnd) && !(S_OK == DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &isCloaked, sizeof(isCloaked)) && isCloaked);
}

/// function that is called by EnumWindows function of WinAPI
static BOOL CALLBACK enumProcess(HWND hwnd, LPARAM lParam) {    
  if (!isWindowVisibleOnScreen(hwnd)) {
    return TRUE;
  }

  std::map<uint32_t, Saiga::Process> *process_list = (std::map<uint32_t, Saiga::Process> *) lParam;

  const int length = GetWindowTextLength(hwnd);
  std::vector<wchar_t> window_title(length + 1);
  
  if (0 == GetWindowTextW(hwnd, window_title.data(), length + 1)) {
    return TRUE;
  }

  DWORD process_id = 0;
  HANDLE process_handler;
  char process_path[MAX_PROCESS_PATH_LEN];
  
  memset(process_path, 0x00, MAX_PROCESS_PATH_LEN); 
  GetWindowThreadProcessId(hwnd, &process_id);
  process_handler = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);

  if (nullptr == process_handler) {
    return TRUE;
  }

  char *process_name = nullptr;
  
  GetModuleFileNameExA(process_handler, nullptr, process_path, sizeof(process_path));
  CloseHandle(process_handler);  
  process_name = PathFindFileNameA(process_path);

  Saiga::Process process;

  process.pid = process_id;
  process.hwnd = (uint64_t)(uintptr_t) hwnd;
  process.name = process_name;
  process.title = converter.to_bytes(window_title.data());
  process.time_tag = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
  process.state = Saiga::ProcessState::CREATED;
  
  process_list->insert({process_id, process});

  return TRUE;
}

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

bool Saiga::ProcessHandler::configure(const std::string &file_path) {
  if (0 == file_path.length()) {
    spdlog::error("could not read configuration file path, because the file path is empty");
    return false;
  }

  std::ifstream config_file(file_path);

  if (!config_file.is_open()) {
    spdlog::error("could not open configuration file {}", file_path);
    return false;
  }
  
  std::stringstream buffer;
  rapidjson::Document document;

  buffer << config_file.rdbuf();
  document.Parse(buffer.str().c_str());

  m_preferences.db_file = document["db_file"].GetString();
  m_preferences.endpoint_config.protocol = (HTTPProtocol) document["secure"].GetUint();
  m_preferences.endpoint_config.ip_address = document["ip_address"].GetString();
  m_preferences.endpoint_config.port_number = document["port_number"].GetUint();
  m_preferences.endpoint_config.extension = document["extension"].GetString();

  config_file.close();
  
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
  std::map<uint32_t, Process> process_list;
  std::map<uint32_t, InstantProcess> instant_process_list;

  // handle process and put into database
  EnumWindows(enumProcess, (LPARAM) &process_list);

  auto it = m_current_process_list.begin();

  while (m_current_process_list.end() != it) {
    if (process_list.end() == process_list.find(it->first)) {
      InstantProcess instant_process;
      int64_t now = duration_cast<seconds>(system_clock::now().time_since_epoch()).count();

      instant_process.name = it->second.name;
      instant_process.duration = now - it->second.time_tag;
      instant_process_list.insert({it->first, instant_process});

      it->second.time_tag = now;
      it->second.state = ProcessState::KILLED;
      m_database_manager->insert(it->second);
      
      it = m_current_process_list.erase(it);
    }
    else {
      ++it;
    }
  }

  for (auto it = process_list.begin(); process_list.end() != it; ++it) {
    if (m_current_process_list.end() == m_current_process_list.find(it->first)) {
      m_database_manager->insert(it->second);
      m_current_process_list.insert({it->first, it->second});
    }
  }

  if (0 < instant_process_list.size()) {
    std::string json_text;

    toJSON(instant_process_list, json_text);
    m_endpoint.transmit(json_text);
   
    spdlog::debug("json text: {}", json_text);
  }
  
  return;
} 

void Saiga::ProcessHandler::toJSON(const std::map<uint32_t, Saiga::InstantProcess> &process_list, std::string &json_text) {
  rapidjson::Value object_list(rapidjson::kArrayType);
  rapidjson::Document document;

  document.SetObject();
  json_text.clear();
  
  rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
  
  for (auto it = process_list.begin(); it != process_list.end(); ++it) {
    rapidjson::Value object(rapidjson::kObjectType);

    object.AddMember("pid", it->first, allocator);
    object.AddMember("name", rapidjson::Value().SetString(it->second.name.c_str(), allocator), allocator);
    object.AddMember("duration", it->second.duration, allocator);

    object_list.PushBack(object, allocator);
  }

  document.AddMember("process_list", object_list, allocator);

  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

  document.Accept(writer);
  json_text = buffer.GetString();

  return;
}
