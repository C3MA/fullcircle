#ifndef LIBFULLCIRCLE_NET_NET_SERVER_HPP
#define LIBFULLCIRCLE_NET_NET_SERVER_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope.hpp>
#include <libfullcircle/net/server_session.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace fullcircle {
  class NetServer 
    : public boost::enable_shared_from_this<NetServer> 
  {
    public:
      typedef boost::shared_ptr<NetServer> Ptr;
      static Ptr create(
          boost::asio::io_service& io_service,
          boost::asio::ip::tcp::endpoint endpoint) {
        return Ptr(new NetServer(io_service, endpoint));
      }

      virtual ~NetServer() {};
      void run();
      void join();
      void shutdown();
      ServerProtocolDispatcher::Ptr getDispatcher();


    private:
      NetServer ( boost::asio::io_service& io_service,
          boost::asio::ip::tcp::endpoint endpoint) 
        : _io_service(io_service)
          , _acceptor(_io_service, endpoint)
          , _t()
    { }
      NetServer (const NetServer& original);
      NetServer& operator= (const NetServer& rhs);
      void do_start();
      void start_accept();
      void handle_accept(ServerSession::Ptr session, 
          const boost::system::error_code& error);
      boost::asio::io_service& _io_service;
      boost::asio::ip::tcp::acceptor _acceptor;
      boost::thread _t;
      ServerProtocolDispatcher::Ptr _dispatcher;
  };
};


#endif 

