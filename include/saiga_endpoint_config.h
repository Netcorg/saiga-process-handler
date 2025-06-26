/**
 * @file endpoint_config.h
 * @brief endpoint configuration type
 */

#pragma once

#include <cstdint>
#include <string>

#define EP_CONFIG_DEF_IP_ADDRESS  ("")
#define EP_CONFIG_DEF_PORT_NUMBER (0U)
#define EP_CONFIG_DEF_EXTENSION   ("saiga-server")
#define EP_CONFIG_DEF_TIMEOUT     (10L)
#define EP_CONFIG_DEF_VERBOSE     (1L)
#define EP_CONFIG_DEF_TOKEN       ("")

namespace Saiga {
  enum HTTPProtocol {
    PROTOCOL_HTTPS,
    PROTOCOL_HTTP
  };

  /// @class endpoint configuration
  class EndpointConfiguration {
  public:
    /// default constructor
    EndpointConfiguration() = default;
    /// destructor
    virtual ~EndpointConfiguration() = default;
    /// function that gets URL which combines attributes
    /// @return URL
    std::string getURL() const;
    /// equality operator overloading
    /// @param [in] config - EndpointConguration instance
    bool operator==(const EndpointConfiguration &config) const;
    /// non-equality operator overloading
    /// @param [in] config - EndpointConguration instance
    bool operator!=(const EndpointConfiguration &config) const;

    /// ip address
    std::string ip_address = EP_CONFIG_DEF_IP_ADDRESS;
    /// port number
    int port_number = EP_CONFIG_DEF_PORT_NUMBER;
    /// page extension
    std::string extension = EP_CONFIG_DEF_EXTENSION;
    /// timeout
    long timeout = EP_CONFIG_DEF_TIMEOUT;
    /// verbose flag
    long verbose = EP_CONFIG_DEF_VERBOSE;
    /// protocol type
    HTTPProtocol protocol = PROTOCOL_HTTPS;
    /// token
    std::string token = EP_CONFIG_DEF_TOKEN;
  };
}
