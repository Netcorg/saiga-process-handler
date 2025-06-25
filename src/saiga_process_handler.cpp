#include "saiga_process_handler.h"
#include <iostream>

Saiga::ProcessHandler::ProcessHandler(const std::string &name)
  : EAR::Schedule::Task(name)
{

}

Saiga::ProcessHandler::~ProcessHandler() {

}

bool Saiga::ProcessHandler::initialize(void) {
  std::cout << "process handler initialized" << std::endl;
  return true;
}


void Saiga::ProcessHandler::cycle(void)  {
  std::cout << "process handler cycle called" << std::endl;
  return;
} 
