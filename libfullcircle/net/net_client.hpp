#ifndef LIBFULLCIRCLE_NET_NET_CLIENT_HPP
#define LIBFULLCIRCLE_NET_NET_CLIENT_HPP 1

#include <libfullcircle/common.hpp>

namespace fullcircle {
  class NetClient {
    public:
      typedef std::tr1::shared_ptr<NetClient> Ptr;
      NetClient (
          const std::string& server,
          const uint16_t& port)
        : _host(server), _port(port) {}
      virtual ~NetClient() {};

    private:
      NetClient (const NetClient& original);
      NetClient& operator= (const NetClient& rhs);
      std::string _host;
      uint16_t port;
  };
};


#endif 

