#include <sstream>
#include "saiga_process.h"

Saiga::Process::Process(void) {

}

Saiga::Process::Process(const Process &process) {
  *this = process;
}

Saiga::Process::Process(Process &&process) {
  *this = std::move(process);
}

Saiga::Process::~Process() {

}

Saiga::Process &Saiga::Process::operator=(const Saiga::Process &process) {
  if (this != &process) {
    pid = process.pid;
    ppid = process.ppid;
    app_name = process.app_name;
    title = process.title;
    time = process.time;
    state = process.state;
  }

  return *this;
}

Saiga::Process &Saiga::Process::operator=(Saiga::Process &&process) {
  if (this != &process) {
    pid = std::move(process.pid);
    ppid = std::move(process.ppid);
    app_name = std::move(process.app_name);
    title = std::move(process.title);
    time = std::move(process.time);
    state = std::move(process.state);
  }

  return *this;
}

bool Saiga::Process::operator==(const Saiga::Process& process) const {
  return pid == process.pid && ppid == process.ppid && 0 == app_name.compare(process.app_name);
}

bool Saiga::Process::operator!=(const Saiga::Process& process) const {
  return !(*this == process);
}

std::string Saiga::Process::toString(void) const {
  std::stringstream ss;

  ss << pid << ", " <<
    ppid << ", " <<
    app_name << ", " <<
    title << ", " <<
    time << ", " <<
    (int) state;

  return ss.str();
}
