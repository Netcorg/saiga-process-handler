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
  Saiga::Process process;
  std::vector<Saiga::Process> process_list;
  
  spdlog::set_level(spdlog::level::debug);

  if (!database_manager->open("db/saiga.db")) {
    spdlog::error("could not open database");
    return -1;
  }

  spdlog::info("database opened");
  
  process.pid = 101;
  process.ppid = 202;
  process.app_name = "Sample Application 2";
  process.title = "Sample Title 2";
  process.time = "123456789";
  process.state = Saiga::ProcessState::CREATED;
  
  if (!database_manager->insert(process)) {
    spdlog::error("could not insert process into database");
  }
  else {
    spdlog::info("process inserted into database");
  }

  if (!database_manager->fetch(process_list)) {
    spdlog::error("could not fetch process list");
  }
  else {
    spdlog::info("process fetched from database");
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
