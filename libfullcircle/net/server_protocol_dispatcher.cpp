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
    std::cout << "Reconstructed snip: " << snip.DebugString() << std::endl;
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
          fullcircle::Snip_RequestSnip request = snip.req_snip();
          _on_request(request);
        }
        break;
      case fullcircle::Snip::FRAME:
        {
          std::cout << "Frame!" << std::endl;
          fullcircle::Snip_FrameSnip frame = snip.frame_snip();
          _on_frame(frame);
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

boost::signals2::connection ServerProtocolDispatcher::do_on_request(
    const on_request_snip_slot_t& slot)
{
  return _on_request.connect(slot);
}

boost::signals2::connection ServerProtocolDispatcher::do_on_frame(
    const on_frame_snip_slot_t& slot)
{
  return _on_frame.connect(slot);
}

void ServerProtocolDispatcher::send_ack() {
  fullcircle::Snip snip;
  snip.set_type(fullcircle::Snip::ACK);
  std::ostringstream oss;
  if (!snip.SerializeToOstream(&oss)) {
    std::cout << "ack snip serialization did not work." << std::endl;
  }
  oss.flush();
  fullcircle::Envelope::Ptr ackenv(new fullcircle::Envelope());
  ackenv->set_body(oss.str());
  _transport->write(ackenv);
}

void ServerProtocolDispatcher::send_nack() {
  fullcircle::Snip snip;
  snip.set_type(fullcircle::Snip::NACK);
  std::ostringstream oss;
  if (!snip.SerializeToOstream(&oss)) {
    std::cout << "nack snip serialization did not work." << std::endl;
  }
  oss.flush();
  fullcircle::Envelope::Ptr nackenv(new fullcircle::Envelope());
  nackenv->set_body(oss.str());
  _transport->write(nackenv);
}

void ServerProtocolDispatcher::send_start() {
  fullcircle::Snip snop;
  snop.set_type(fullcircle::Snip::START);
  std::ostringstream oss2;
  if (!snop.SerializeToOstream(&oss2)) {
    std::cout << "start snip serialization did not work." << std::endl;
  }
  oss2.flush();
  fullcircle::Envelope::Ptr renv(new fullcircle::Envelope());
  renv->set_body(oss2.str());
  _transport->write(renv);
}

void ServerProtocolDispatcher::send_abort() {
  fullcircle::Snip snip;
  snip.set_type(fullcircle::Snip::ABORT);
  std::ostringstream oss2;
  if (!snip.SerializeToOstream(&oss2)) {
    std::cout << "start snip serialization did not work." << std::endl;
  }
  oss2.flush();
  fullcircle::Envelope::Ptr renv(new fullcircle::Envelope());
  renv->set_body(oss2.str());
  _transport->write(renv);
}
