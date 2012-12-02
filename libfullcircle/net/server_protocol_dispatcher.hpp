#ifndef LIBFULLCIRCLE_NET_SERVER_PROTOCOL_DISPATCHER_HPP
#define LIBFULLCIRCLE_NET_SERVER_PROTOCOL_DISPATCHER_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope_transport.hpp>
#include <libfullcircle/sequence.pb.h>
#include <boost/signals2.hpp>
#include <boost/system/error_code.hpp>

namespace fullcircle {
  class ServerProtocolDispatcher {
    public:
      typedef boost::shared_ptr<ServerProtocolDispatcher> Ptr;
      ServerProtocolDispatcher(fullcircle::EnvelopeTransport::Ptr transport) 
        : _transport(transport)
        , _active(true) {};
      virtual ~ServerProtocolDispatcher() {};

      void handle_envelope(fullcircle::Envelope::Ptr env);
      void handle_error(boost::system::error_code error);

      typedef boost::signals2::signal<void (fullcircle::Snip_RequestSnip)>     on_request_snip_t;
      typedef on_request_snip_t::slot_type on_request_snip_slot_t;
      boost::signals2::connection do_on_request(const on_request_snip_slot_t& slot);

      typedef boost::signals2::signal<void (fullcircle::Snip_FrameSnip)>     on_frame_snip_t;
      typedef on_frame_snip_t::slot_type on_frame_snip_slot_t;
      boost::signals2::connection do_on_frame(const on_frame_snip_slot_t& slot);

      typedef boost::signals2::signal<void (fullcircle::Snip_TimeoutSnip)>     on_timeout_snip_t;
      typedef on_timeout_snip_t::slot_type on_timeout_snip_slot_t;
      boost::signals2::connection do_on_timeout(const on_timeout_snip_slot_t& slot);

      typedef boost::signals2::signal<void (fullcircle::Snip_AbortSnip)>     on_abort_snip_t;
      typedef on_abort_snip_t::slot_type on_abort_snip_slot_t;
      boost::signals2::connection do_on_abort(const on_abort_snip_slot_t& slot);

      typedef boost::signals2::signal<void (fullcircle::Snip_EosSnip)>     on_eos_snip_t;
      typedef on_eos_snip_t::slot_type on_eos_snip_slot_t;
      boost::signals2::connection do_on_eos(const on_eos_snip_slot_t& slot);

      void send_ack();
      void send_nack();
      void send_start();
      void send_abort();
      void send_timeout();
      bool is_active();

    private:
      ServerProtocolDispatcher (const ServerProtocolDispatcher& original);
      ServerProtocolDispatcher& operator= (const ServerProtocolDispatcher& rhs);
      fullcircle::EnvelopeTransport::Ptr _transport;
      bool _active;
      on_request_snip_t _on_request;
      on_frame_snip_t _on_frame;
      on_timeout_snip_t _on_timeout;
      on_abort_snip_t _on_abort;
      on_eos_snip_t _on_eos;

  };

};


#endif /* LIBFULLCIRCLE_NET_SERVER_PROTOCOL_DISPATCHER_HPP */

