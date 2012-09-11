#ifndef LIBFULLCIRCLE_NET_NET_CLIENT_HPP
#define LIBFULLCIRCLE_NET_NET_CLIENT_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

namespace fullcircle {
  class NetClient {
    public:
      typedef std::tr1::shared_ptr<NetClient> Ptr;
      NetClient (
          boost::asio::io_service& io_service,
          boost::asio::ip::tcp::resolver::iterator endpoint_iterator
        );
      virtual ~NetClient() {};
      void run();
      void shutdown();
      void write(Envelope::Ptr envelope);

    private:
      NetClient (const NetClient& original);
      NetClient& operator= (const NetClient& rhs);
      void do_close();
      void handle_connect(const boost::system::error_code& error);
      void handle_read_header(const boost::system::error_code& error);
      void handle_read_body(const boost::system::error_code& error);
      void do_write(Envelope::Ptr msg);
      void handle_write(const boost::system::error_code& error);

      boost::asio::io_service& _io_service;
      boost::asio::ip::tcp::socket _socket;
      boost::asio::ip::tcp::resolver::iterator _endpoint_iterator;
      boost::thread _t;
      Envelope::Ptr _read_envelope;
      envelope_queue_t _write_envelopes;
  };
};


#endif 

