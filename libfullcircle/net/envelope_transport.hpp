#ifndef LIBFULLCIRCLE_NET_ENVELOPE_TRANSPORT_HPP
#define LIBFULLCIRCLE_NET_ENVELOPE_TRANSPORT_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/net/envelope.hpp>
#include <boost/shared_ptr.hpp>

namespace fullcircle {
  class EnvelopeTransport {
    public:
      typedef boost::shared_ptr<EnvelopeTransport> Ptr;
      EnvelopeTransport () {};
      virtual ~EnvelopeTransport() {};

      virtual void write(Envelope::Ptr envelope) = 0; 

    private:
      EnvelopeTransport (const EnvelopeTransport& original);
      EnvelopeTransport& operator= (const EnvelopeTransport& rhs);
      
  };
};


#endif /* LIBFULLCIRCLE_NET_ENVELOPE_TRANSPORT_HPP */

