#ifndef LIBFULLCIRCLE_SERVER_TCPCONNECTION_HPP
#define LIBFULLCIRCLE_SERVER_TCPCONNECTION_HPP 1

#include <ctime>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/unordered_map.hpp>
#include "server_client_handle.hpp"

namespace fullcircle {
	
	using boost::asio::ip::tcp;
	typedef boost::unordered_map<std::string, ClientHandle::Ptr> MapOfClients; 
	
	class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
	{
	public:
		typedef boost::shared_ptr<tcp_connection> Ptr;
		tcp_connection(boost::asio::io_service& io_service, ClientHandle::Ptr handle);
		
		static Ptr create(boost::asio::io_service& io_service, ClientHandle::Ptr handle)
		{
			return Ptr(new tcp_connection(io_service, handle));
		}
		
		tcp::socket& socket()
		{ 
			return _socket;
		}
		
		void handle_read(const boost::system::error_code &ec, std::size_t bytes_transferred);
		void handle_write(const boost::system::error_code& /*error*/, size_t bytes_transferred);
		
		void start();
		std::string get_client();
		
	private:
		ClientHandle::Ptr		 _handle;
		boost::array<char, 4096> _buffer;
		tcp::socket _socket;
		std::string _message;
	};
	
	class tcp_server
	{
	public:
		tcp_server(boost::asio::io_service& io_service);
		uint32_t get_other_registered_clients();
	private:
		void start_accept(ClientHandle::Ptr handle);
		
		void handle_accept(tcp_connection::Ptr new_connection, const boost::system::error_code& error);
		
		tcp::acceptor _acceptor;
		MapOfClients  _clients;
	};
};


#endif /* LIBFULLCIRCLE_SERVER_TCPCONNECTION_HPP */