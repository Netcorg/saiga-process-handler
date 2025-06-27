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
  /// filtered process type that is visualized
  class FilteredProcess {
  public:
    std::string name;
    uint32_t session_count = 0U;
    uint32_t duration = 0U;
  };
  
  class DatabaseManager {
  public:
    /// function that gets class instance
    /// @return class instance
    static DatabaseManager *getInstance(void);
    /// destructor
    virtual ~DatabaseManager();
    /// function that opens database
    /// @param [in] db_file - database file path
    /// @return true if the database is opened successfully, otherwise false
    bool open(const std::string &db_file);
    /// function that closes database
    /// @return true if the database is closed successfully, otherwise false
    bool close(void);
    /// function that insert a Process reference into the table
    /// @parma [in] process - Process reference
    /// @return true if process is inserted into the table, otherwise false
    bool insert(const Process &process);
    /// function that insert a Process reference into the table
    /// @parma [in] process - Process reference
    /// @return true if process is inserted into the table, otherwise false
    bool insert(const std::vector<Process> &process_list);
    /// function that fetches process list that is limited by start and stop time tags, then convert it to process list
    /// @param [out] process_list - database process list
    /// @parma [in] start_time - start time for process into the table
    /// @parma [in] end_time - end time for process into the table
    /// @return true if process list is fetched, otherwise false
    bool fetch(std::vector<FilteredProcess> &process_list, const uint32_t start_time, const uint32_t end_time);
   
  private:
    /// class instance
    static DatabaseManager *m_instance;
    /// database reference
    sqlite3 *m_database = nullptr;

    /// default constructor
    DatabaseManager(void);
  };
}
