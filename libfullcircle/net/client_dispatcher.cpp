#include "client_dispatcher.hpp"
#include <sstream>


using namespace fullcircle;

void ClientDispatcher::handle_envelope(
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
      case fullcircle::Snip::PONG: 
        {
          fullcircle::Snip_PongSnip pong=snip.pong_snip();
          //std::cout << "Received ping reply (no. " 
          //  << pong.count() << ")" << std::endl;
          _on_pong(pong);
        }
        break;
      case fullcircle::Snip::ERROR: 
        {
          fullcircle::Snip_ErrorSnip error=snip.error_snip();
          _on_error(error);
        }
        break;
      default: 
        std::cout << "Unknown snip, discarding." << std::endl;
        break;
    }
  }

}

void ClientDispatcher::send_ping(const uint16_t seq_id) {
  fullcircle::Snip snip;
  snip.set_type(fullcircle::Snip::PING);
  fullcircle::Snip_PingSnip* ping=snip.mutable_ping_snip();
  ping->set_count(seq_id);
  std::ostringstream oss2;
  if (!snip.SerializeToOstream(&oss2)) {
    std::cout << "ping snip serialization did not work." << std::endl;
  }
  oss2.flush();
  fullcircle::Envelope::Ptr env(new fullcircle::Envelope());
  env->set_body(oss2.str());
  _transport->write(env);
}

boost::signals2::connection ClientDispatcher::do_on_pong(
    const on_pong_snip_slot_t& slot) 
{
  return _on_pong.connect(slot);
}

boost::signals2::connection ClientDispatcher::do_on_error(
    const on_error_snip_slot_t& slot) 
{
  return _on_error.connect(slot);
}
