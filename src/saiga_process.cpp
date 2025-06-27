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
    hwnd = process.hwnd;
    time_tag = process.time_tag;
    name = process.name;
    title = process.title;
    state = process.state;
  }

  return *this;
}

Saiga::Process &Saiga::Process::operator=(Saiga::Process &&process) {
  if (this != &process) {
    pid = std::move(process.pid);
    hwnd = std::move(process.hwnd);
    time_tag = std::move(process.time_tag);
    name = std::move(process.name);
    title = std::move(process.title);
    state = std::move(process.state);
  }

  return *this;
}

bool Saiga::Process::operator==(const Saiga::Process& process) const {
  return pid == process.pid && hwnd == process.hwnd && 0 == name.compare(process.name);
}

bool Saiga::Process::operator!=(const Saiga::Process& process) const {
  return !(*this == process);
}

std::string Saiga::Process::toString(void) const {
  std::stringstream ss;

  ss << pid << ", " <<
    hwnd << ", " <<
    title << ", " <<
    name << ", " <<
    time_tag << ", " <<
    (int) state;

  return ss.str();
}
