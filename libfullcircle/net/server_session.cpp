#include "server_session.hpp"
#include <boost/bind.hpp>

using namespace fullcircle;


ServerSession::ServerSession(boost::asio::io_service& io_service)
  : _socket(io_service)
  , _read_envelope(new Envelope())
  , _write_envelopes()
{
  std::cout << "ServerSession: Constructed instance " <<
    this << std::endl;
}

ServerSession::~ServerSession() {
  std::cout << "ServerSession: Destructing instance " <<
    this << std::endl;
}

boost::asio::ip::tcp::socket& ServerSession::socket() {
  return _socket;
}

void ServerSession::start() {
  std::cout << "ServerSession: Starting in instance " << this << std::endl;
  try {
      boost::asio::async_read(_socket,
        boost::asio::buffer(_read_envelope->get_raw_ptr(), 
          Envelope::header_length),
        boost::bind(
          &ServerSession::handle_read_header, shared_from_this(),
          boost::asio::placeholders::error));

  } catch (const std::exception& e) {
    std::cout << "ServerSession: Failed to start, " 
      << e.what() << std::endl;
  }
}


void ServerSession::handle_read_header(
    const boost::system::error_code& error) 
{
  if (!error && _read_envelope->decode_header()) {
    std::cout << "ServerSession: Reading header." << std::endl;
    boost::asio::async_read(_socket,
        boost::asio::buffer(
          _read_envelope->get_body_ptr(), 
          _read_envelope->get_body_length()),
        boost::bind(&ServerSession::handle_read_body, shared_from_this(),
          boost::asio::placeholders::error));
  } else {
    if (error) {
      std::cout << "ServerSession: Cannot read header: " 
        << error.message() << std::endl;
    }
  }

}

void ServerSession::handle_read_body(const boost::system::error_code& error)
{
  if (!error) {
    std::cout << "ServerSession: Reading body succeeded: " 
      << _read_envelope->str()  << std::endl;
    boost::asio::async_read(_socket,
        boost::asio::buffer(_read_envelope->get_raw_ptr(), 
          Envelope::header_length),
        boost::bind(&ServerSession::handle_read_header, shared_from_this(),
          boost::asio::placeholders::error));
  } else {
    std::cout << "ServerSession: Cannot read body: " 
      << error.message() << std::endl;
  }

}

void ServerSession::write(Envelope::Ptr envelope) {
  do_write(envelope);
}

void ServerSession::do_write(Envelope::Ptr envelope) {
  bool write_in_progress = !_write_envelopes.empty();
  _write_envelopes.push_back(envelope);
  if (!write_in_progress) {
    boost::asio::async_write(_socket,
        boost::asio::buffer(_write_envelopes.front()->get_raw_ptr(),
          _write_envelopes.front()->get_raw_length()),
        boost::bind(&ServerSession::handle_write, shared_from_this(),
          boost::asio::placeholders::error));
  }
}

void ServerSession::handle_write(const boost::system::error_code& error) 
{
  if (!error) {
    std::cout << "ServerSession: Handling write" << std::endl;
    _write_envelopes.pop_front();
    if (!_write_envelopes.empty()) {
      boost::asio::async_write(_socket,
          boost::asio::buffer(_write_envelopes.front()->get_raw_ptr(),
            _write_envelopes.front()->get_raw_length()),
          boost::bind(&ServerSession::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
    }
  } else {
    std::cout << "ServerSession: Cannot handle write: " 
      << error.message() << std::endl;
  }
}


