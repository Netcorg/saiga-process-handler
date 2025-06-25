#include <unistd.h>
#include "saiga_process_handler.h"
#include "sched/scheduler.h"
#include "spdlog/spdlog.h"

int main() {
  EAR::Schedule::Scheduler scheduler("saiga-scheduler");
  Saiga::ProcessHandler process_handler("saiga-process-handler");

  spdlog::set_level(spdlog::level::debug);
  
  scheduler.allocate(&process_handler, 1000000U, 0U);
  scheduler.start();
  sleep(10);
  scheduler.stop();
  
  return 0;
}
