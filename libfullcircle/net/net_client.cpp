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
  _socket.close();
}

void NetClient::write(Envelope::Ptr envelope) {
  //io_service_.post(boost::bind(&chat_client::do_write, this, envelope));
}

void NetClient::handle_connect(const boost::system::error_code& error)
{
  if (!error) {
    std::cout << "Client: Connection succeeded." << std::endl;
    //
    //    boost::asio::async_read(_socket,
    //        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
    //        boost::bind(&chat_client::handle_read_header, this,
    //          boost::asio::placeholders::error));
  }
}




