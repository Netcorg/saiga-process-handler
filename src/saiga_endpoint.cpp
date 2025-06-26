#include "saiga_endpoint.h"
#include "spdlog/spdlog.h"

Saiga::Endpoint::Endpoint(void) {

}

Saiga::Endpoint::~Endpoint() {

}

void Saiga::Endpoint::setConfiguration(const Saiga::EndpointConfiguration &config) {
  m_config = config;
  return;
}

const Saiga::EndpointConfiguration &Saiga::Endpoint::getConfiguration() const {
  return m_config;
}

bool Saiga::Endpoint::initialize(void) {
  if (0 == m_config.ip_address.length() || 0U == m_config.port_number) {
    spdlog::error("invalid endpoint configuration");
    return false;
  }

  bool is_connected = false;

  do {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    m_curl = curl_easy_init();

    if (nullptr == m_curl) {
      std::this_thread::sleep_for(std::chrono::seconds(m_config.timeout));
    }
    else {
      curl_easy_setopt(m_curl, CURLOPT_URL, m_config.getURL().c_str());
      curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, m_config.timeout);
      curl_easy_setopt(m_curl, CURLOPT_VERBOSE, m_config.verbose);

      if (CURLE_OK != curl_easy_perform(m_curl)) {
        std::this_thread::sleep_for(std::chrono::seconds(m_config.timeout));
      }
      else {
        m_header = curl_slist_append(m_header, "Content-Type: application/json");

        if (!m_config.token.empty()) {
          std::string bearer_header = "Authorization: Bearer " + m_config.token;
          m_header = curl_slist_append(m_header, bearer_header.c_str());
       }

        curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, m_header);
        curl_easy_setopt(m_curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);

        if (PROTOCOL_HTTPS == m_config.protocol) {
          /// @todo verifypeer to 1, verifyhost to 2 for server that has SSL certificate
          curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
          curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);
        }

        is_connected = true;
      }
    }
  } while (!is_connected);

  return is_connected;
}

bool Saiga::Endpoint::terminate(void) {
  curl_slist_free_all(m_header);
  curl_easy_cleanup(m_curl);
  curl_global_cleanup();

  return true;
}

bool Saiga::Endpoint::transmit(const std::string &data) {
  CURLcode res;
  
  curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data.c_str());
  res = curl_easy_perform(m_curl);
    
  if (CURLE_OK != res) {
    spdlog::error("curl_easy_perform failed: {}", curl_easy_strerror(res));
    return false;
  }

  return true;
}

bool Saiga::Endpoint::operator==(const Saiga::Endpoint &ep) const {
  return m_config == ep.m_config;
}

bool Saiga::Endpoint::operator!=(const Saiga::Endpoint &ep) const {
  return !(*this == ep);
}
