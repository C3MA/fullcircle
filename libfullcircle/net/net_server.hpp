#ifndef LIBFULLCIRCLE_NET_NET_SERVER_HPP
#define LIBFULLCIRCLE_NET_NET_SERVER_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope.hpp>
#include <libfullcircle/net/server_session.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

namespace fullcircle {
    class NetServer {
    public:
      typedef std::tr1::shared_ptr<NetServer> Ptr;
      NetServer (
          boost::asio::io_service& io_service,
          boost::asio::ip::tcp::endpoint endpoint
        ) : _io_service(io_service)
          , _acceptor(_io_service, endpoint)
          , _t()
      {
        start_accept();
      }
      virtual ~NetServer() {};
      void run();
      void shutdown();


    private:
      NetServer (const NetServer& original);
      NetServer& operator= (const NetServer& rhs);
      void start_accept();
      void handle_accept(ServerSession::Ptr session, 
          const boost::system::error_code& error);
      boost::asio::io_service& _io_service;
      boost::asio::ip::tcp::acceptor _acceptor;
      boost::thread _t;
    };
};


#endif 

