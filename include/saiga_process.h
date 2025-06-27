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
    /// function that converts Process instance to string
    /// @return instance in string format
    std::string toString(void) const;
    
    /// process id
    uint32_t pid = 0;
    /// window handler
    uint64_t hwnd = 0U;
    /// time tag when state changes
    int64_t time_tag = -1;
    /// name
    std::string name;
    /// title
    std::string title;
    /// state
    ProcessState state = ProcessState::NONE;
  };
}
