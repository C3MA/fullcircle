#ifndef LIBFULLCIRCLE_NET_CLIENT_DISPATCHER_HPP
#define LIBFULLCIRCLE_NET_CLIENT_DISPATCHER_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope_transport.hpp>
#include <libfullcircle/net/envelope.hpp>
#include <libfullcircle/sequence.pb.h>
#include <libfullcircle/frame.hpp>
#include <boost/signals2.hpp>


namespace fullcircle {
  class ClientDispatcher {
    public:
      typedef boost::shared_ptr<ClientDispatcher> Ptr;

      ClientDispatcher ( fullcircle::EnvelopeTransport::Ptr transport) 
        : _transport(transport) 
          , _on_pong()
          , _on_error()
	  , _on_ack()
	  , _on_nack()
	  , _on_start()
    {};
      virtual ~ClientDispatcher() {};

      void send_ping(const uint16_t seq_id);


      /**
       * Send a request in order to get a timmeslot to send dynamic information
       * @param color Each input-station has a unique color
       * @param seq_id unique id for every interaction
       * @param meta Meta information
       */
      void send_request(std::string color, uint32_t seq_id, fullcircle::BinarySequenceMetadata* meta);
      void send_frame(fullcircle::Frame::Ptr frame);
      void handle_envelope(fullcircle::Envelope::Ptr env);

      /**
       * Callbacks for incoming packets. Calling these allows you to
       * register behaviour for each packet type individually.
       */
      typedef boost::signals2::signal<void (fullcircle::Snip_PongSnip)>        on_pong_snip_t;
      typedef on_pong_snip_t::slot_type on_pong_snip_slot_t;
      boost::signals2::connection do_on_pong(const on_pong_snip_slot_t& slot);

      typedef boost::signals2::signal<void (fullcircle::Snip_ErrorSnip)>        on_error_snip_t;
      typedef on_error_snip_t::slot_type on_error_snip_slot_t;
      boost::signals2::connection do_on_error(const on_error_snip_slot_t& slot);

      typedef boost::signals2::signal<void (fullcircle::Snip_AckSnip)>        on_ack_snip_t;
      typedef on_ack_snip_t::slot_type on_ack_snip_slot_t;
      boost::signals2::connection do_on_ack(const on_ack_snip_slot_t& slot);

      typedef boost::signals2::signal<void (fullcircle::Snip_NackSnip)>        on_nack_snip_t;
      typedef on_nack_snip_t::slot_type on_nack_snip_slot_t;
      boost::signals2::connection do_on_nack(const on_nack_snip_slot_t& slot);

      typedef boost::signals2::signal<void (fullcircle::Snip_StartSnip)>        on_start_snip_t;
      typedef on_start_snip_t::slot_type on_start_snip_slot_t;
      boost::signals2::connection do_on_start(const on_start_snip_slot_t& slot);

    private:
      ClientDispatcher (const ClientDispatcher& original);
      ClientDispatcher& operator= (const ClientDispatcher& rhs);
      fullcircle::EnvelopeTransport::Ptr _transport;
      on_pong_snip_t _on_pong;
      on_error_snip_t _on_error;
      on_ack_snip_t _on_ack;
      on_nack_snip_t _on_nack;
      on_start_snip_t _on_start;
  };

};


#endif /* LIBFULLCIRCLE_NET_CLIENT_DISPATCHER_HPP */

