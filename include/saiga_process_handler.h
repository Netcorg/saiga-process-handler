/**
 * @file saiga_process_handler.h
 * @brief application class of saiga process handler
 */

#pragma once

#include <map>
#include "sched/task.h"
#include "saiga_process.h"
#include "saiga_database_manager.h"
#include "saiga_endpoint.h"

namespace Saiga {
  /// process handler application class
  class ProcessHandler : public EAR::Schedule::Task {
  public:
    /// constructor
    /// @param [in] name - task name
    explicit ProcessHandler(const std::string &name);
    /// destructor
    virtual ~ProcessHandler();
    /// function that configures process handler via configuration json
    /// @param [in] config_file - json configuration file
    /// @return true if the configuration is done successfully, otherwise false
    bool configure(const std::string &config_file);
    /// function that initializes process handler
    /// @remark it is called by scheduler once automatically
    virtual bool initialize(void) override;
    /// function that performs cycle operation of process handler
    /// @remark it is called periodically by scheduler after allocation into main
    virtual void cycle(void) override;

  private:
    /// current process list
    std::map<uint32_t, Process> m_current_process_list;
    /// databaase manager
    DatabaseManager *m_database_manager;
    /// endpoint
    Endpoint m_endpoint;

    /// function that converts filtered process list to json object
    /// @param [in] process_list - filtered process list
    /// @param [out] json_text - json output
    void toJSON(const std::vector<FilteredProcess> &process_list, std::string &json_text);
    /// function that handles alive or dead process at OS level
    /// @param [in] process_list - process list
    void handleProcess(std::vector<Process> &process_list);
  };
}
