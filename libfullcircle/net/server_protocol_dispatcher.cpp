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
    throw fullcircle::DataFormatException("Cannot load snip from input stream.");
  } else {
    //std::cout << "Reconstructed snip: " << snip.DebugString() << std::endl;
    switch (snip.type()) {
      case fullcircle::Snip::PING: 
        {
          fullcircle::Snip_PingSnip ping=snip.ping_snip();
          std::cout << "Responding to ping (no. " << ping.count() << ")" << std::endl;
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
      case fullcircle::Snip::REQUEST:
        {
          std::cout << "Request!" << std::endl;
	// kommunikation mit sceduler um zu checken von Ressourcen bereit gesetllt sind!
        }
        break;
      case fullcircle::Snip::FRAME:
        {
          std::cout << "Frame!" << std::endl;
        }
        break;
      case fullcircle::Snip::TIMEOUT:
        {
          std::cout << "Timeout!" << std::endl;
        }
        break;
      default: 
        std::cout << "Unknown snip, discarding." << std::endl;
        break;
    }
  }

}
