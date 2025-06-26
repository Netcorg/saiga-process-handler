#pragma once

#include <string>
#include <vector>
#include "sqlite3.h"
#include "saiga_process.h"

namespace Saiga {
  class DatabaseManager {
  public:
    static DatabaseManager *getInstance(void);
    virtual ~DatabaseManager();
    bool open(const std::string &db_file);
    bool close(void);
    bool insert(const Process &process);
    /// @todo add condition as parameter
    bool fetch(std::vector<Process> &process_list);
    
  private:
    static DatabaseManager *m_instance;
    sqlite3 *m_database = nullptr;

    /// default constructor
    DatabaseManager(void);
  };
}
