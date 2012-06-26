#include "server_tcpconnection.hpp"
#include <iostream>
#include <boost/format.hpp>

using boost::asio::ip::tcp;
using namespace fullcircle;

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}

void tcp_connection::handle_read(const boost::system::error_code &ec, std::size_t bytes_transferred) 
{ 
	if (!ec) 
	{ 
		std::cerr << get_client() << " | received: " << std::string(_buffer.data(), bytes_transferred) << std::endl;			
		_socket.async_read_some( boost::asio::buffer(_buffer), 
								 boost::bind(&tcp_connection::handle_read, shared_from_this(),
											 boost::asio::placeholders::error,
											 boost::asio::placeholders::bytes_transferred));
	} else if (ec == boost::asio::error::eof) {
		std::cerr << get_client() << " | left." << std::endl;
	} else {
		std::cerr << get_client() << " | Could not read incoming data." << std::endl;
	}
}

void tcp_connection::start()
{
	_message = make_daytime_string();
	_socket.async_read_some( boost::asio::buffer(_buffer), 
							 boost::bind(&tcp_connection::handle_read, shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
	// on a new created socket, write a answer with the time
	boost::asio::async_write(_socket, boost::asio::buffer(_message),
							 boost::bind(&tcp_connection::handle_write, shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

std::string tcp_connection::get_client()
{	
	if (!_socket.is_open())
	{
		return "<Nobody>";
	}
	else
	{
		return str( boost::format("%1%:%2%") % 
				   _socket.remote_endpoint().address().to_string() % 
				   _socket.remote_endpoint().port());
	}
}


void tcp_connection::handle_write(const boost::system::error_code& /*error*/, size_t bytes_transferred)
{
	std::cerr << get_client() << " | " << bytes_transferred << " bytes written" << std::endl;
	_socket.async_read_some( boost::asio::buffer(_buffer), 
							 boost::bind(&tcp_connection::handle_read, shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
} 

tcp_server::tcp_server(boost::asio::io_service& io_service): _acceptor(io_service, tcp::endpoint(tcp::v4(), 2342))
{
	ClientHandle::Ptr dummy(new ClientHandle("dummy", this));
	start_accept(dummy);
}

void tcp_server::start_accept(ClientHandle::Ptr handle)
{
	tcp_connection::Ptr new_connection = tcp_connection::create(_acceptor.get_io_service(), handle);
	_acceptor.async_accept(new_connection->socket(),
						   boost::bind(&tcp_server::handle_accept, this, new_connection,
									   boost::asio::placeholders::error));
}

void tcp_server::handle_accept(tcp_connection::Ptr new_connection, const boost::system::error_code& error)
{
	if (!error)
	{
		new_connection->start();
		std::cerr << new_connection->get_client() << " | A new visitor arrived, other: " << _clients.size() << std::endl;
		
		// add this client to the hashmap
		ClientHandle::Ptr new_handle(new ClientHandle(new_connection->get_client(), this));			
		_clients[new_handle->get_key()] = new_handle; 
		start_accept(new_handle);
	} else {
		ClientHandle::Ptr dummy(new ClientHandle("dummy", this));
		start_accept(dummy);
	}
	
	
}
