#include "server_session.hpp"
#include <boost/bind.hpp>
#include <libfullcircle/sequence.pb.h>

using namespace fullcircle;

ServerSession::ServerSession(boost::asio::io_service& io_service)
  : _socket(io_service)
  , _read_envelope(new Envelope())
  , _write_envelopes()
{}

ServerSession::~ServerSession() 
{
  std::cout << "Server: Deleting session " << this << std::endl;
}

ServerSession::Ptr ServerSession::create(
    boost::asio::io_service& io_service) 
{
  Ptr retval(new ServerSession(io_service));
  return retval;
}


void ServerSession::write(Envelope::Ptr envelope) {
  do_write(envelope);
}

boost::signals2::connection ServerSession::do_on_envelope(
    const on_envelope_slot_t& slot)
{
  return _on_envelope.connect(slot);
}

boost::signals2::connection ServerSession::do_on_error(
    const on_error_slot_t& slot)
{
  return _on_error.connect(slot);
}

boost::asio::ip::tcp::socket& ServerSession::socket() {
  return _socket;
}

void ServerSession::start() {
  std::cout << "ServerSession: Starting session " << this << std::endl;
  try {
    boost::asio::async_read(_socket,
        boost::asio::buffer(_read_envelope->get_raw_ptr(), 
          Envelope::header_length),
        boost::bind(
          &ServerSession::handle_read_header, shared_from_this(),
          boost::asio::placeholders::error));

  } catch (const std::exception& e) {
    std::cerr << "ServerSession: Failed to start, " 
      << e.what() << std::endl;
  }
}

void ServerSession::stop() {
  std::cout << "ServerSession: Terminating session " << this << std::endl;
  _socket.close();
}

void ServerSession::handle_connection_error(
    const boost::system::error_code& error,
    const std::string& additional_hint
    ) 
{
  std::cerr << "ServerSession: " << additional_hint <<
    ", error: " << error.message() << std::endl;
  _on_error(error);
}


void ServerSession::handle_read_header(
    const boost::system::error_code& error) 
{
  if (!error && _read_envelope->decode_header()) {
//    std::cout << "ServerSession: Reading header." << std::endl;
    boost::asio::async_read(_socket,
        boost::asio::buffer(
          _read_envelope->get_body_ptr(), 
          _read_envelope->get_body_length()),
        boost::bind(&ServerSession::handle_read_body, shared_from_this(),
          boost::asio::placeholders::error));
  } else {
    if (error) {
      handle_connection_error(error, "can't read header");
    }
  }

}

void ServerSession::handle_read_body(const boost::system::error_code& error)
{
  if (!error) {
    try {
      _on_envelope(_read_envelope);
    } catch (const std::exception& e) {
      std::ostringstream oss;
      oss << "ServerSession: Exception during processing of envelope: " 
        << e.what() << " Sending error snip back to client." << std::endl;
      std::cerr << oss.str() << std::endl;
      fullcircle::Snip error;
      error.set_type(fullcircle::Snip::ERROR);
      fullcircle::Snip_ErrorSnip* esnip=error.mutable_error_snip();
      esnip->set_errorcode(fullcircle::Snip::DECODING_ERROR);
      esnip->set_description(oss.str());
      std::ostringstream oss2;
      if (!error.SerializeToOstream(&oss2)) {
        std::cout << "error snip serialization did not work." << std::endl;
      }
      oss2.flush();
      fullcircle::Envelope::Ptr renv(new fullcircle::Envelope());
      renv->set_body(oss2.str());
      do_write(renv);
    }
    // react to the next message of this client.
    boost::asio::async_read(_socket,
        boost::asio::buffer(_read_envelope->get_raw_ptr(), 
          Envelope::header_length),
        boost::bind(&ServerSession::handle_read_header, shared_from_this(),
          boost::asio::placeholders::error));
  } else {
    handle_connection_error(error, "can't read body");
  }
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
    //std::cout << "ServerSession: Handling write" << std::endl;
    _write_envelopes.pop_front();
    if (!_write_envelopes.empty()) {
      boost::asio::async_write(_socket,
          boost::asio::buffer(_write_envelopes.front()->get_raw_ptr(),
            _write_envelopes.front()->get_raw_length()),
          boost::bind(&ServerSession::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
    }
  } else {
    handle_connection_error(error, "can't write");
  }
}


