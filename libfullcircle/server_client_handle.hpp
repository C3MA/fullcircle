#ifndef LIBFULLCIRCLE_SERVER_CLIENT_HANDLE_HPP
#define LIBFULLCIRCLE_SERVER_CLIENT_HANDLE_HPP 1

#include <string.h>
#include <boost/shared_ptr.hpp>

namespace fullcircle {

  //define a dummy tcp server here, so we can make a back reference (no include in header, only in source)
  class tcp_server;
	
  class ClientHandle {
    public:
      typedef boost::shared_ptr<ClientHandle> Ptr;

      ClientHandle(std::string ipAddrPort, tcp_server* server);
      ClientHandle (ClientHandle& rhs);
      virtual ~ClientHandle() {};
	  std::string get_key() { return _key; }
    private:
	  uint8_t	  _state;
	  std::string _key;
	  tcp_server* _server;
  };
};


#endif /* LIBFULLCIRCLE_FRAME_HPP */

