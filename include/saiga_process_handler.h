#include "sched/task.h"

namespace Saiga {
  class ProcessHandler : public EAR::Schedule::Task {
  public:
    explicit ProcessHandler(const std::string &name);
    virtual ~ProcessHandler();
    
    virtual bool initialize(void) override;
    virtual void cycle(void) override;
  };
}
