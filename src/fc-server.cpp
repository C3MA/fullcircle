#include <libfullcircle/net/net_server.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

int main (int argc, char const* argv[]) {
  std::cout << "FullCircle Test Server starting up." << std::endl;
  boost::asio::io_service server_io_service;
  boost::asio::ip::tcp::endpoint server_endpoint(
      boost::asio::ip::tcp::v4(), 24567);
  fullcircle::NetServer::Ptr server = fullcircle::NetServer::create(
      server_io_service, server_endpoint);
  server->run();
  server->join();

  return 0;
}

