#include "server_client_handle.hpp"
#include <iostream>


using namespace fullcircle;


ClientHandle::ClientHandle( std::string ipAddrPort) : _key(ipAddrPort)
{
	
}
