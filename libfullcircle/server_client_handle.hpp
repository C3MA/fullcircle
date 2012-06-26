#ifndef LIBFULLCIRCLE_SERVER_CLIENT_HANDLE_HPP
#define LIBFULLCIRCLE_SERVER_CLIENT_HANDLE_HPP 1

#include <string.h>
#include <boost/shared_ptr.hpp>

namespace fullcircle {

  class ClientHandle {
    public:
      typedef boost::shared_ptr<ClientHandle> Ptr;

      ClientHandle(std::string ipAddrPort);
      ClientHandle (ClientHandle& rhs);
      virtual ~ClientHandle() {};
	  std::string get_key() { return _key; }
    private:
	  uint8_t	  _state;
	  std::string _key;
  };
};


#endif /* LIBFULLCIRCLE_FRAME_HPP */

