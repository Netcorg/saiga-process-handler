#include "saiga_endpoint_config.h"

std::string Saiga::EndpointConfiguration::getURL() const {
  if (HTTPProtocol::NORMAL == protocol) {
    return "http://" + ip_address + ":" + std::to_string(port_number) + "/" + extension;
  }
  else {
    return "https://" + ip_address + ":" + std::to_string(port_number) + "/" + extension;
  }
}

bool Saiga::EndpointConfiguration::operator==(const Saiga::EndpointConfiguration &config) const {
  return 0 == ip_address.compare(config.ip_address) &&
    port_number == config.port_number &&
    0 == extension.compare(config.extension);
}

bool Saiga::EndpointConfiguration::operator!=(const Saiga::EndpointConfiguration &config) const {
  return !(*this == config);
}
