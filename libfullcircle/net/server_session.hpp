#ifndef LIBFULLCIRCLE_NET_SERVER_SESSION_HPP
#define LIBFULLCIRCLE_NET_SERVER_SESSION_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

namespace fullcircle {
  class ServerSession 
    : public boost::enable_shared_from_this<ServerSession> 
  {
    public:
      typedef std::tr1::shared_ptr<ServerSession> Ptr;
      static Ptr create(boost::asio::io_service& io_service) {
        return Ptr(new ServerSession(io_service));
      }
      virtual ~ServerSession();

      void start();
      boost::asio::ip::tcp::socket& socket();
      void write(Envelope::Ptr envelope);

      void do_write(Envelope::Ptr envelope);
      void handle_write(const boost::system::error_code& error);
      void handle_read_header(const boost::system::error_code& error);
      void handle_read_body(const boost::system::error_code& error);
    private:
      ServerSession(boost::asio::io_service& io_service);
      ServerSession (const ServerSession& original);
      ServerSession& operator= (const ServerSession& rhs);

      boost::asio::ip::tcp::socket _socket;
      Envelope::Ptr _read_envelope;
      envelope_queue_t _write_envelopes;

  };

};


#endif /* LIBFULLCIRCLE_NET_SERVER_SESSION_HPP */

