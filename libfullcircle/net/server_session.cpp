#include "server_session.hpp"
#include <boost/bind.hpp>

using namespace fullcircle;


ServerSession::ServerSession(boost::asio::io_service& io_service)
  : _socket(io_service)
  , _read_envelope(new Envelope())
{
}

boost::asio::ip::tcp::socket& ServerSession::socket() {
  return _socket;
}

void ServerSession::start() {
  std::cout << "session start" << std::endl;
  try {
  boost::asio::async_read(_socket,
      boost::asio::buffer(_read_envelope->get_raw_ptr(), 
        Envelope::header_length),
      boost::bind(
        &ServerSession::handle_read_header, this, // shared_from_this(),
        boost::asio::placeholders::error));
  } catch (const std::exception& e) {
    std::cout << "Fu: " << e.what() << std::endl;
  }
}

void ServerSession::deliver(Envelope::Ptr envelope) {
  std::cout << "session deliver" << std::endl; 
}
void ServerSession::handle_read_header(const boost::system::error_code& error) 
{
  std::cout << "session read_header" << std::endl;
}
void ServerSession::handle_read_body(const boost::system::error_code& error)
{
  std::cout << "session read_body" << std::endl;
}

void ServerSession::handle_read_write(const boost::system::error_code& error) 
{
  std::cout << "session read_write" << std::endl;
}


