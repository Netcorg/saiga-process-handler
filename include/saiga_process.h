#pragma once

#include <cstdint>
#include <string>

namespace Saiga {
  /// Saiga process state list
  enum class ProcessState {
    NONE,
    CREATED,
    KILLED
  };

  class Process {
  public:
    /// default constructor
    Process(void);
    /// copy constructor
    /// @param [in] process - Process reference
    Process(const Process &process);
    /// move constructor
    /// @param [in] process - Process reference
    Process(Process &&process);
    /// destructor
    virtual ~Process();
    /// assignment operator overloading
    /// @param [in] Process - Process reference
    /// @return *this
    Process &operator=(const Process &process);
    /// move assignment operator overloading
    /// @param [in] Process - Process reference
    /// @return *this
    Process &operator=(Process &&process);
    /// equal operator overloading
    /// @remark app_name is mandatory field for matching, but name is optional if empty
    /// @param [in] Process - Process instance
    /// @return true if two objects are same, otherwise false
    bool operator==(const Process& process) const;
    /// not equal operator overloading
    /// @remark app_name is mandatory field for matching, but name is optional if empty
    /// @param [in] Process - Process instance
    /// @return true if two objects are not same, otherwise false
    bool operator!=(const Process& process) const;
    
    /// process id
    uint32_t pid = 0;
    /// parent process id
    uint32_t ppid = 0;
    /// application name
    std::string app_name;
    /// title
    std::string title;
    /// time
    std::string time;
    /// state
    ProcessState state = ProcessState::NONE;
  };
}
