#ifndef LIBFULLCIRCLE_NET_CLIENT_DISPATCHER_HPP
#define LIBFULLCIRCLE_NET_CLIENT_DISPATCHER_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope_transport.hpp>
#include <libfullcircle/net/envelope.hpp>
#include <libfullcircle/sequence.pb.h>
#include <boost/signals2.hpp>


namespace fullcircle {
  class ClientDispatcher {
    public:
      typedef boost::shared_ptr<ClientDispatcher> Ptr;

      ClientDispatcher ( fullcircle::EnvelopeTransport::Ptr transport) 
        : _transport(transport) 
        , _on_pong()
        , _on_error()
      {};
      virtual ~ClientDispatcher() {};

      void send_ping(const uint16_t seq_id);
      void handle_envelope(fullcircle::Envelope::Ptr env);

      /**
       * Callbacks for incoming packets. Calling these allows you to
       * register behaviour for each packet type individually.
       */
      typedef boost::signals2::signal<void (fullcircle::Snip_PongSnip)> 
        on_pong_snip_t;
      typedef on_pong_snip_t::slot_type on_pong_snip_slot_t;
      boost::signals2::connection do_on_pong(const on_pong_snip_slot_t& slot);

      typedef boost::signals2::signal<void (fullcircle::Snip_ErrorSnip)> 
        on_error_snip_t;
      typedef on_error_snip_t::slot_type on_error_snip_slot_t;
      boost::signals2::connection do_on_error(const on_error_snip_slot_t& slot);

    private:
      ClientDispatcher (const ClientDispatcher& original);
      ClientDispatcher& operator= (const ClientDispatcher& rhs);
      fullcircle::EnvelopeTransport::Ptr _transport;
      on_pong_snip_t _on_pong;
      on_error_snip_t _on_error;
      
  };
  
};


#endif /* LIBFULLCIRCLE_NET_CLIENT_DISPATCHER_HPP */
