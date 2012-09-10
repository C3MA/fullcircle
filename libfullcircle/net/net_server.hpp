#ifndef LIBFULLCIRCLE_NET_NET_SERVER_HPP
#define LIBFULLCIRCLE_NET_NET_SERVER_HPP 1

#include <libfullcircle/common.hpp>

namespace fullcircle {
  class NetServer {
    public:
      typedef std::tr1::shared_ptr<NetServer> Ptr;
      NetServer ( const uint16_t& port)
        : _port(port) {}
      virtual ~NetServer() {};

    private:
      NetServer (const NetServer& original);
      NetServer& operator= (const NetServer& rhs);
      uint16_t port;
  };
};


#endif 

