#include "net_server.hpp"
#include <boost/bind.hpp>

using namespace fullcircle;

void NetServer::run() {
  _t = boost::thread(
      boost::bind(&NetServer::do_start, this));
      //boost::bind(&boost::asio::io_service::run, &_io_service));
  //start_accept();
}

void NetServer::do_start() {
  try {
    start_accept();
    _io_service.run();
  } catch (const std::exception& e) {
    std::cout << "Server: do_start failed, " << e.what() << std::endl;
  }
}

void NetServer::shutdown() {
  std::cout << "Server is shutting down." << std::endl;
  _io_service.stop();
  _t.join();
}

void NetServer::handle_accept(ServerSession::Ptr session, 
    const boost::system::error_code& error) 
{
  std::cout << "Server: handle_accept with " << session.get() 
    <<", use-count: " << session.use_count() << std::endl;
  if (!error) {
    session->start();
  } else {
    std::cout << "Server: Cannot handle accept: " <<
      error.message() << std::endl;
  }

  start_accept();
}

void NetServer::start_accept() {
  ServerSession::Ptr new_session = ServerSession::create(_io_service);
  ServerProtocolDispatcher::Ptr dispatcher(
      new ServerProtocolDispatcher(new_session));
  new_session->do_on_envelope(
      boost::bind(&ServerProtocolDispatcher::handle_envelope, dispatcher, _1));
  std::cout << "Server: start_accept, use_count: "
    << new_session.use_count() << std::endl;
  _acceptor.async_accept(new_session->socket(),
      boost::bind(
        &NetServer::handle_accept, this, 
        new_session, boost::asio::placeholders::error));
}

