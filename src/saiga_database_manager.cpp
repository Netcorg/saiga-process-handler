#include "saiga_database_manager.h"
#include "spdlog/spdlog.h"

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
  
  return SQLITE_OK == sqlite3_open(db_file.c_str(), &m_database);
}

bool Saiga::DatabaseManager::close(void) {
  if (nullptr == m_database) {
    spdlog::error("could not close database that is not opened yet");
    return false;
  }

  return SQLITE_OK == sqlite3_close(m_database);
}

bool Saiga::DatabaseManager::insert(const Saiga::Process &process) {
  return false;
}

bool Saiga::DatabaseManager::fetch(std::vector<Saiga::Process> &process_list) {
  return false;
}
