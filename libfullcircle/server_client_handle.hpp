#ifndef LIBFULLCIRCLE_SERVER_CLIENT_HANDLE_HPP
#define LIBFULLCIRCLE_SERVER_CLIENT_HANDLE_HPP 1

#include <string.h>
#include <boost/shared_ptr.hpp>

namespace fullcircle {

  //define a dummy tcp server here, so we can make a back reference (no include in header, only in source)
  class tcp_server;
	
  class ClientHandle {
  public:
	  enum State {ONLINE, REGISTERED, WRITING, KICKED};
      typedef boost::shared_ptr<ClientHandle> Ptr;

      ClientHandle(std::string ipAddrPort, tcp_server* server);
      ClientHandle (ClientHandle& rhs);
      virtual ~ClientHandle() {};
	  std::string get_key() { return _key; }
	  bool	is_active();
	  void	parse(std::string message);
  private:
	  bool	is_writing();
	  
	  State	  _state;
	  std::string _key;
	  tcp_server* _server;
  };
};


#endif /* LIBFULLCIRCLE_SERVER_CLIENT_HANDLE_HPP */
