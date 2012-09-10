#include "net_server.hpp"
#include <boost/bind.hpp>

using namespace fullcircle;

void NetServer::run() {
  _t = boost::thread(
      boost::bind(&boost::asio::io_service::run, &_io_service));
}

void NetServer::shutdown() {
  std::cout << "Server is shutting down." << std::endl;
  _io_service.stop();
  _t.join();
}

void NetServer::handle_accept(ServerSession::Ptr session, 
    const boost::system::error_code& error) 
{
  std::cout << "server handle_accept" << std::endl;
  if (!error) {
    session->start();
  } else {
    std::cout << "Invalid session, discarding." << std::endl;
  }

  start_accept();
}

void NetServer::start_accept() {
  std::cout << "server start_accept" << std::endl;
  ServerSession::Ptr new_session(new ServerSession(_io_service));
    _acceptor.async_accept(new_session->socket(),
        boost::bind(&NetServer::handle_accept, this, new_session,
          boost::asio::placeholders::error));
 
}

