//
// server.cpp
// ~~~~~~~~~~
//

// This is a small workaround to get the source easily compiled with XCode.
// to get this work, the following constant has to be defined. I assume it not present with CMake
#ifdef __XCODE_USED__ 
#include "server_tcpconnection.hpp"
#else
#include "libfullcircle/server_tcpconnection.hpp"
#endif

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
