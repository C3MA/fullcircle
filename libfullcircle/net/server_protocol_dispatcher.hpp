#ifndef LIBFULLCIRCLE_NET_SERVER_PROTOCOL_DISPATCHER_HPP
#define LIBFULLCIRCLE_NET_SERVER_PROTOCOL_DISPATCHER_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope_transport.hpp>

namespace fullcircle {
  class ServerProtocolDispatcher {
    public:
      typedef boost::shared_ptr<ServerProtocolDispatcher> Ptr;
      ServerProtocolDispatcher(fullcircle::EnvelopeTransport::Ptr transport) 
        : _transport(transport) {};
      virtual ~ServerProtocolDispatcher() {};

      void handle_envelope(fullcircle::Envelope::Ptr env);

    private:
      ServerProtocolDispatcher (const ServerProtocolDispatcher& original);
      ServerProtocolDispatcher& operator= (const ServerProtocolDispatcher& rhs);
      fullcircle::EnvelopeTransport::Ptr _transport;
      
  };
  
};


#endif /* LIBFULLCIRCLE_NET_SERVER_PROTOCOL_DISPATCHER_HPP */

