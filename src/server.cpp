//
// server.cpp
// ~~~~~~~~~~
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
public:
	typedef boost::shared_ptr<tcp_connection> pointer;
	
	static pointer create(boost::asio::io_service& io_service)
	{
		return pointer(new tcp_connection(io_service));
	}
	
	tcp::socket& socket()
	{
		return socket_;
	}
	
	void handle_read(const boost::system::error_code &ec, std::size_t bytes_transferred) 
	{ 
		if (!ec) 
		{ 
			std::cout << "received: " << std::string(a_buffer.data(), bytes_transferred) << std::endl;			
			socket_.async_read_some( boost::asio::buffer(a_buffer), 
									boost::bind(&tcp_connection::handle_read, shared_from_this(),
												boost::asio::placeholders::error,
												boost::asio::placeholders::bytes_transferred));
		} else {
			std::cerr << "Could not read incoming data." << std::endl;
		}
	}
	
	void start()
	{
		message_ = make_daytime_string();
		socket_.async_read_some( boost::asio::buffer(a_buffer), 
								boost::bind(&tcp_connection::handle_read, shared_from_this(),
											boost::asio::placeholders::error,
											boost::asio::placeholders::bytes_transferred));
		// on a new created socket, write a answer with the time
		boost::asio::async_write(socket_, boost::asio::buffer(message_),
								 boost::bind(&tcp_connection::handle_write, shared_from_this(),
											 boost::asio::placeholders::error,
											 boost::asio::placeholders::bytes_transferred));
	}
	
	std::string get_client_ip()
	{
		return "FIXME";//FIXME get the ip address of the connected client
	}
	
private:
	tcp_connection(boost::asio::io_service& io_service)    : socket_(io_service)
	{
	}
	
	void handle_write(const boost::system::error_code& /*error*/,
					  size_t bytes_transferred)
	{
		std::cout << "" << bytes_transferred << " bytes written" << std::endl;
		socket_.async_read_some( boost::asio::buffer(a_buffer), 
								boost::bind(&tcp_connection::handle_read, shared_from_this(),
											boost::asio::placeholders::error,
											boost::asio::placeholders::bytes_transferred));
	} 
	
	boost::array<char, 4096> a_buffer;
	tcp::socket socket_;
	std::string message_;
};

class tcp_server
{
public:
	tcp_server(boost::asio::io_service& io_service): acceptor_(io_service, tcp::endpoint(tcp::v4(), 2342))
	{
		start_accept();
	}
	
private:
	void start_accept()
	{
		tcp_connection::pointer new_connection =
		tcp_connection::create(acceptor_.get_io_service());
		
		acceptor_.async_accept(new_connection->socket(),
							   boost::bind(&tcp_server::handle_accept, this, new_connection,
										   boost::asio::placeholders::error));
	}
	
	void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
	{
		if (!error)
		{
			new_connection->start();
		}
		
		start_accept();
	}
	
	tcp::acceptor acceptor_;
};

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
