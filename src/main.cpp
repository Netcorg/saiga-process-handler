#include "sched/scheduler.h"
#include "spdlog/spdlog.h"
#include "saiga_process_handler.h"

#ifdef WIN32
#include <windows.h>
#define SAIGA_SLEEP(sec) Sleep((sec) * 1000)
#else
#include <unistd.h>
#define SAIGA_SLEEP(sec) sleep((sec))
#endif

int main() {
  EAR::Schedule::Scheduler scheduler("saiga-scheduler");
  Saiga::ProcessHandler process_handler("saiga-process-handler");
  
  spdlog::set_level(spdlog::level::debug);
  scheduler.allocate(&process_handler, 1000000U, 0U);

  scheduler.start(); 
  SAIGA_SLEEP(2);
  scheduler.stop();
  
  return 0;
}
