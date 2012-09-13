#ifndef LIBFULLCIRCLE_NET_NET_CLIENT_HPP
#define LIBFULLCIRCLE_NET_NET_CLIENT_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope.hpp>
#include <libfullcircle/net/envelope_transport.hpp>
#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/thread/thread.hpp>

namespace fullcircle {
  class NetClient
    : public fullcircle::EnvelopeTransport
  {
    public:
      typedef boost::shared_ptr<NetClient> Ptr;
      typedef boost::signals2::signal<void (fullcircle::Envelope::Ptr)> on_envelope_t;
      typedef on_envelope_t::slot_type on_envelope_slot_t;
      NetClient (
          boost::asio::io_service& io_service,
          boost::asio::ip::tcp::resolver::iterator endpoint_iterator
        );
      virtual ~NetClient() {};
      void run();
      void shutdown();
      void write(Envelope::Ptr envelope);

      boost::signals2::connection do_on_envelope(const on_envelope_slot_t& slot);


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
      on_envelope_t _on_envelope;
  };
};


#endif 

