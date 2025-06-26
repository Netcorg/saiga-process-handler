#include "saiga_process_handler.h"
#include "sched/scheduler.h"
#include "saiga_database_manager.h"
#include "spdlog/spdlog.h"

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
  Saiga::DatabaseManager *database_manager = Saiga::DatabaseManager::getInstance();
  std::vector<Saiga::DatabaseEntry> entry_list;
  
  spdlog::set_level(spdlog::level::debug);

  if (!database_manager->open("db/saiga.db")) {
    spdlog::error("could not open database");
    return -1;
  }

  spdlog::info("database opened");
  
  if (!database_manager->fetch(entry_list, 0, 999999999)) {
    spdlog::error("could not fetch process list");
  }
  else {
    spdlog::info("entries fetched from database");
  }

  scheduler.allocate(&process_handler, 1000000U, 0U);

  scheduler.start(); 
  SAIGA_SLEEP(2);
  scheduler.stop();

  if (!database_manager->close()) {
    spdlog::error("could not close database");
    return -1;
  }

  spdlog::info("database closed");
  
  return 0;
}
