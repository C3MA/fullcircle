#ifndef LIBFULLCIRCLE_NET_CLIENT_DISPATCHER_HPP
#define LIBFULLCIRCLE_NET_CLIENT_DISPATCHER_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope_transport.hpp>
#include <libfullcircle/net/envelope.hpp>


namespace fullcircle {
  class ClientDispatcher {
    public:
      typedef boost::shared_ptr<ClientDispatcher> Ptr;
      ClientDispatcher ( fullcircle::EnvelopeTransport::Ptr transport) 
        : _transport(transport) {};
      virtual ~ClientDispatcher() {};

      void send_ping(const uint16_t seq_id);
      void handle_envelope(fullcircle::Envelope::Ptr env);

    private:
      ClientDispatcher (const ClientDispatcher& original);
      ClientDispatcher& operator= (const ClientDispatcher& rhs);
      fullcircle::EnvelopeTransport::Ptr _transport;
      
  };
  
};


#endif /* LIBFULLCIRCLE_NET_CLIENT_DISPATCHER_HPP */

