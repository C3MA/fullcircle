#include "net_client.hpp"

using namespace fullcircle;

NetClient::NetClient(
  boost::asio::io_service& io_service,
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator
) 
  : _io_service(io_service)
  , _socket(_io_service)
  , _endpoint_iterator(endpoint_iterator)
  , _t()
  , _read_envelope(new Envelope())
  , _write_envelopes()
{
}

void NetClient::run() {
  std::cout << "Running client." << std::endl;
  boost::asio::async_connect(_socket, _endpoint_iterator,
        boost::bind(&NetClient::handle_connect, this,
          boost::asio::placeholders::error));
  _t = boost::thread(
      boost::bind(&boost::asio::io_service::run, &_io_service));
}

void NetClient::shutdown() {
  std::cout << "Client is shutting down." << std::endl;
  _io_service.post(boost::bind(&NetClient::do_close, this));
  //_io_service.stop();
  _t.join();
}

void NetClient::do_close() {
  std::cout << "Client: Closing socket." << std::endl;
  _socket.close();
}

void NetClient::write(Envelope::Ptr envelope) {
  _io_service.post(boost::bind(&NetClient::do_write, this, envelope));
}

void NetClient::handle_connect(const boost::system::error_code& error) {
  if (!error) {
    std::cout << "Client: Connection succeeded." << std::endl;

    boost::asio::async_read(_socket,
        boost::asio::buffer(
          _read_envelope->get_raw_ptr(), Envelope::header_length),
        boost::bind(&NetClient::handle_read_header, this,
          boost::asio::placeholders::error));
  } else {
    std::cout << "Client: Connection failed: " 
      << error.message() << std::endl;
    //do_close();
  }
}

void NetClient::handle_read_header(const boost::system::error_code& error) {
  if (!error && _read_envelope->decode_header()) {
    std::cout << "Client: Reading header succeeded." << std::endl;
    boost::asio::async_read(_socket,
        boost::asio::buffer(
          _read_envelope->get_body_ptr(), 
          _read_envelope->get_body_length()),
        boost::bind(&NetClient::handle_read_body, this,
          boost::asio::placeholders::error));
  } else {
    std::cout << "Client: Reading header failed: "
      << error.message() << std::endl;
    //do_close();
  }
}

void NetClient::handle_read_body(const boost::system::error_code& error) {
  if (!error) {
    std::cout << "Client: Received envelope " 
      << _read_envelope->str() << std::endl;
    boost::asio::async_read(_socket,
        boost::asio::buffer(
          _read_envelope->get_raw_ptr(), Envelope::header_length),
        boost::bind(&NetClient::handle_read_header, this,
          boost::asio::placeholders::error));
  } else {
    std::cout << "Client: Receiving body failed: "
      << error.message() << std::endl;
    //do_close();
  }
}

void NetClient::do_write(Envelope::Ptr msg) {
  bool write_in_progress = !_write_envelopes.empty();
  _write_envelopes.push_back(msg);
  if (!write_in_progress) {
    boost::asio::async_write(_socket,
        boost::asio::buffer(_write_envelopes.front()->get_raw_ptr(),
          _write_envelopes.front()->get_raw_length()),
        boost::bind(&NetClient::handle_write, this,
          boost::asio::placeholders::error));
  }
}

void NetClient::handle_write(const boost::system::error_code& error) {
  if (!error) {
    _write_envelopes.pop_front();
    if (!_write_envelopes.empty()) {
      boost::asio::async_write(_socket,
        boost::asio::buffer(_write_envelopes.front()->get_raw_ptr(),
          _write_envelopes.front()->get_raw_length()),
        boost::bind(&NetClient::handle_write, this,
            boost::asio::placeholders::error));
    }
  } else {
    std::cout << "Client: Cannot send envelopes: " 
      << error.message() << std::endl;
    //do_close();
  }
}

