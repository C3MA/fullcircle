#ifndef LIBFULLCIRCLE_SERVER_TCPCONNECTION_HPP
#define LIBFULLCIRCLE_SERVER_TCPCONNECTION_HPP 1

#include <ctime>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>

namespace fullcircle {
	
	using boost::asio::ip::tcp;
	
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
			return a_socket;
		}
		
		void handle_read(const boost::system::error_code &ec, std::size_t bytes_transferred);
		void handle_write(const boost::system::error_code& /*error*/, size_t bytes_transferred);
		
		void start();
	private:
		tcp_connection(boost::asio::io_service& io_service)    : a_socket(io_service)
		{
			std::cerr << get_client() << " | A new visitor arrived " << std::endl; 
		}
		
		std::string get_client();
		
		boost::array<char, 4096> a_buffer;
		tcp::socket a_socket;
		std::string a_message;
	};
	
	class tcp_server
	{
	public:
		tcp_server(boost::asio::io_service& io_service);
	private:
		void start_accept();
		
		void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error);
		
		tcp::acceptor a_acceptor;
	};
};


#endif /* LIBFULLCIRCLE_SERVER_TCPCONNECTION_HPP */