#include "net_server.hpp"
#include <boost/bind.hpp>

using namespace fullcircle;

void NetServer::run() {
  _t = boost::thread(
      boost::bind(&NetServer::do_start, this));
      //boost::bind(&boost::asio::io_service::run, &_io_service));
  //start_accept();
}

void NetServer::join() {
  _t.join();
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
    _dispatcher->setSession(session);
  } else {
    std::cout << "Server: Cannot handle accept: " <<
      error.message() << std::endl;
  }


  /* dirty hack, that can probalbly work */
  if (_config == "")
	_config = _dispatcher->getConfig();
  else
	_dispatcher->setConfig(_config);

  start_accept();
}

void NetServer::start_accept() {
  ServerSession::Ptr new_session = ServerSession::create(_io_service);
  _dispatcher.reset(
      new ServerProtocolDispatcher(new_session, _io_service));
  // Link signals to slots.
  new_session->do_on_envelope(
      boost::bind(&ServerProtocolDispatcher::handle_envelope, _dispatcher, _1));
  new_session->do_on_error(
      boost::bind(&ServerProtocolDispatcher::handle_error, _dispatcher, _1));
  std::cout << "Server: start_accept, use_count: "
    << new_session.use_count() << std::endl;
  _acceptor.async_accept(new_session->socket(),
      boost::bind(
        &NetServer::handle_accept, this, 
        new_session, boost::asio::placeholders::error));
}

ServerProtocolDispatcher::Ptr NetServer::getDispatcher() {
	while ( _dispatcher == NULL )
		usleep(100);
	return _dispatcher;
}
