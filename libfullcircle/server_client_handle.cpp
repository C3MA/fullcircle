#include "server_client_handle.hpp"
#include "server_tcpconnection.hpp"
#include <boost/algorithm/string.hpp>
#include <iostream>


using namespace fullcircle;
using namespace std;
using namespace boost::algorithm;

ClientHandle::ClientHandle( std::string ipAddrPort, tcp_server* server) : _key(ipAddrPort), _server(server)
{
	_state = ONLINE;
}

bool ClientHandle::is_active()
{
	return (_state == REGISTERED);
}

void ClientHandle::parse(string message)
{
	trim(message);
	if (message == "REGISTER" && _state == ONLINE) {
		cout << "du kommst hier net REIN!" << endl;
		uint32_t other = _server->get_other_registered_clients();
		if (other == 0) {
			_state = REGISTERED;
		} else {
			/* say the little client, that he is not alone */
		}

	}
}

bool ClientHandle::is_writing()
{
	//FIXME add here a search in the tcp_server, who is actual writing data to the logic.
	return 1;
}
