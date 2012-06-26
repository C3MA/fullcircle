//
// server.cpp
// ~~~~~~~~~~
//

#include "libfullcircle/server_tcpconnection.hpp"

using namespace fullcircle;

int main()
{
	try
	{
		boost::asio::io_service io_service;
		tcp_server server(io_service);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	return 0;
}
