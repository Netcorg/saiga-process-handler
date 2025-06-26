/**
 * @file saiga_database_manager.h
 * @brief database manager class
 * @todo add function(s) that fetch range desired to time, or other columns
 */

#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"
#include "saiga_process.h"

namespace Saiga {
  class DatabaseEntry {
  public:
    std::string app_name;
    uint32_t session_count = 0U;
    uint32_t duration = 0U;
  };
  
  class DatabaseManager {
  public:
    static DatabaseManager *getInstance(void);
    virtual ~DatabaseManager();
    bool open(const std::string &db_file);
    bool close(void);
    bool insert(const Process &process);
    bool fetch(std::vector<DatabaseEntry> &entry_list, const uint32_t start_time, const uint32_t end_time);
   
  private:
    static DatabaseManager *m_instance;
    sqlite3 *m_database = nullptr;

    /// default constructor
    DatabaseManager(void);
  };
}
