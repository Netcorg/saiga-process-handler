#include "saiga_process_handler.h"
#include "spdlog/spdlog.h"

Saiga::ProcessHandler::ProcessHandler(const std::string &name)
  : EAR::Schedule::Task(name)
{

}

Saiga::ProcessHandler::~ProcessHandler() {

}

bool Saiga::ProcessHandler::initialize(void) {
  spdlog::debug("process handler initialized");
  return true;
}


void Saiga::ProcessHandler::cycle(void)  {
  spdlog::debug("process handler cycle called");
  return;
} 
