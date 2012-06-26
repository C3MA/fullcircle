#include "server_client_handle.hpp"
#include "server_tcpconnection.hpp"
#include <iostream>


using namespace fullcircle;


ClientHandle::ClientHandle( std::string ipAddrPort, tcp_server* server) : _key(ipAddrPort), _server(server)
{
	
}
