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

  if (!process_handler.configure("config/config.json")) {
    spdlog::error("could not configure process handler");
    return -1;
  }
  
  scheduler.allocate(&process_handler, 1000000U, 0U);

  scheduler.start(); 
  SAIGA_SLEEP(60);
  scheduler.stop();
  
  return 0;
}
