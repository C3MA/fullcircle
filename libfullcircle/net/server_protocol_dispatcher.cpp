#include "server_protocol_dispatcher.hpp"
#include <sstream>
#include <libfullcircle/sequence.pb.h>


using namespace fullcircle;

void ServerProtocolDispatcher::handle_envelope(
    fullcircle::Envelope::Ptr env) 
{
  // create snip from envelope.
  std::istringstream iss(env->get_body());
  fullcircle::Snip snip;
  if (!snip.ParseFromIstream(&iss)) {
    std::cout << "Cannot load snip from input stream." << std::endl;
  } else {
    //std::cout << "Reconstructed snip: " << snip.DebugString() << std::endl;
    switch (snip.type()) {
      case fullcircle::Snip::PING: 
        {
          std::cout << "Responding to ping snip." << std::endl;
          fullcircle::Snip_PingSnip ping=snip.ping_snip();
          fullcircle::Snip snop;
          snop.set_type(fullcircle::Snip::PONG);
          fullcircle::Snip_PongSnip* pong=snop.mutable_pong_snip();
          pong->set_count(ping.count());
          std::ostringstream oss;
          if (!snop.SerializeToOstream(&oss)) {
            std::cout << "pong snip serialization did not work." << std::endl;
          }
          oss.flush();
          fullcircle::Envelope::Ptr renv(new fullcircle::Envelope());
          renv->set_body(oss.str());
          _transport->write(renv);
        }
        break;
      default: 
        std::cout << "Unknown snip, discarding." << std::endl;
        break;
    }
  }

}
