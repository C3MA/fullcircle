#ifndef LIBFULLCIRCLE_NET_SERVER_SESSION_HPP
#define LIBFULLCIRCLE_NET_SERVER_SESSION_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope.hpp>
#include <libfullcircle/net/envelope_transport.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/signals2.hpp>

/**
 * The session just extracts the snip from the envelope and provides a
 * callback to the dispatcher. We use a signal/slot-mechanism to bind
 * the dispatcher to the session. See also
 * http://stackoverflow.com/questions/768351/complete-example-using-boostsignals-for-c-eventing
 */

namespace fullcircle {
  class ServerSession 
    : public boost::enable_shared_from_this<ServerSession> 
    , public fullcircle::EnvelopeTransport
  {
    public:
      typedef boost::shared_ptr<ServerSession> Ptr;
      typedef boost::signals2::signal<void (fullcircle::Envelope::Ptr)> 
        on_envelope_t;
      typedef on_envelope_t::slot_type on_envelope_slot_t;
      typedef boost::signals2::signal<void (boost::system::error_code)> 
        on_error_t;
      typedef on_error_t::slot_type on_error_slot_t;
      static Ptr create(boost::asio::io_service& io_service);
      virtual ~ServerSession();

      void start();
      void stop();
      boost::asio::ip::tcp::socket& socket();
      void write(Envelope::Ptr envelope);
      boost::signals2::connection do_on_envelope(const on_envelope_slot_t& slot);
      boost::signals2::connection do_on_error(const on_error_slot_t& slot);

    private:
      ServerSession(boost::asio::io_service& io_service);
      ServerSession (const ServerSession& original);
      ServerSession& operator= (const ServerSession& rhs);
      
      void do_write(Envelope::Ptr envelope);
      void handle_write(const boost::system::error_code& error);
      void handle_read_header(const boost::system::error_code& error);
      void handle_read_body(const boost::system::error_code& error);

      void handle_connection_error(
          const boost::system::error_code& error,
          const std::string& additional_hint);

      boost::asio::ip::tcp::socket _socket;
      Envelope::Ptr _read_envelope;
      envelope_queue_t _write_envelopes;
      on_envelope_t _on_envelope;
      on_error_t _on_error;

  };

};


#endif /* LIBFULLCIRCLE_NET_SERVER_SESSION_HPP */

