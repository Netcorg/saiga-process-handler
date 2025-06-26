/**
 * @file endpoint.h
 * @brief endpoint class that is used to transfer activity data to server via REST API
 */

#pragma once

#include "curl/curl.h"
#include "saiga_endpoint_config.h"

namespace Saiga {
  /// @class endpoint
  class Endpoint {
  public:
    /// constructor
    Endpoint(void);
    /// destructor
    virtual ~Endpoint();
    /// function that initializes endpoint
    /// @param [in] config - endpoint configuration instance
    /// @return true if initialization is done successfully, otherwise false
    bool initialize(const EndpointConfiguration &config);
    /// function that terminates endpoint
    /// @return true if termination is done successfully, otherwise false
    bool terminate(void);
    /// function that transmits data to server
    /// @return true if transmission is done successfully, otherwise false
    bool transmit(const std::string &data);
    /// equality operator overloading
    /// @param [in] ep - Endpoint instance
    bool operator==(const Endpoint &ep) const;
    /// non-equality operator overloading
    /// @param [in] ep - Endpoint instance
    bool operator!=(const Endpoint &ep) const;

  private:
    /// cURL instance
    CURL* m_curl = nullptr;
    /// cURL header instance
    curl_slist* m_header = nullptr;
  };
}
