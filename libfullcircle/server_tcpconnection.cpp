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
		std::cerr << get_client() << " | received: " << std::string(a_buffer.data(), bytes_transferred) << std::endl;			
		a_socket.async_read_some( boost::asio::buffer(a_buffer), 
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
	a_message = make_daytime_string();
	a_socket.async_read_some( boost::asio::buffer(a_buffer), 
							 boost::bind(&tcp_connection::handle_read, shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
	// on a new created socket, write a answer with the time
	boost::asio::async_write(a_socket, boost::asio::buffer(a_message),
							 boost::bind(&tcp_connection::handle_write, shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
}

std::string tcp_connection::get_client()
{	
	if (!a_socket.is_open())
	{
		return "<Nobody>";
	}
	else
	{
		return str( boost::format("%1%:%2%") % 
				   a_socket.remote_endpoint().address().to_string() % 
				   a_socket.remote_endpoint().port());
	}
}


void tcp_connection::handle_write(const boost::system::error_code& /*error*/, size_t bytes_transferred)
{
	std::cerr << get_client() << " | " << bytes_transferred << " bytes written" << std::endl;
	a_socket.async_read_some( boost::asio::buffer(a_buffer), 
							 boost::bind(&tcp_connection::handle_read, shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
} 

tcp_server::tcp_server(boost::asio::io_service& io_service): a_acceptor(io_service, tcp::endpoint(tcp::v4(), 2342))
{
	start_accept();
}

void tcp_server::start_accept()
{
	tcp_connection::pointer new_connection = tcp_connection::create(a_acceptor.get_io_service());
	a_acceptor.async_accept(new_connection->socket(),
						   boost::bind(&tcp_server::handle_accept, this, new_connection,
									   boost::asio::placeholders::error));
}

void tcp_server::handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
{
	if (!error)
	{
		new_connection->start();
	}
	
	start_accept();
}
