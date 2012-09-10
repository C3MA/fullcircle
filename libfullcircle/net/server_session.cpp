#include "server_session.hpp"
#include <boost/bind.hpp>

using namespace fullcircle;


ServerSession::ServerSession(boost::asio::io_service& io_service)
  : _socket(io_service) 
{
}

boost::asio::ip::tcp::socket& ServerSession::socket() {
  return _socket;
}

void ServerSession::start() {
  std::cout << "start" << std::endl;
  boost::asio::async_read(_socket,
      boost::asio::buffer(_read_envelope->get_raw_ptr(), 
        Envelope::header_length),
      boost::bind(
        &ServerSession::handle_read_header, shared_from_this(),
        boost::asio::placeholders::error));

}

void ServerSession::deliver(Envelope::Ptr envelope) {
  std::cout << "deliver" << std::endl; 
}
void ServerSession::handle_read_header(const boost::system::error_code& error) 
{
  std::cout << "read_header" << std::endl;
}
void ServerSession::handle_read_body(const boost::system::error_code& error)
{
  std::cout << "read_body" << std::endl;
}

void ServerSession::handle_read_write(const boost::system::error_code& error) 
{
  std::cout << "read_write" << std::endl;
}


