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
          _timer.cancel();
          std::cout << "Frame!" << std::endl;
          fullcircle::Snip_FrameSnip frame = snip.frame_snip();
          _on_frame(frame);
          _timer.expires_from_now(boost::posix_time::seconds(1));
          _timer.async_wait(boost::bind(&ServerProtocolDispatcher::timeout, this, _1));
        }
        break;
      case fullcircle::Snip::TIMEOUT:
        {
          std::cout << "Timeout!" << std::endl;
          fullcircle::Snip_TimeoutSnip timeout = snip.timeout_snip();
          _on_timeout_snip(timeout);
          _active = false;
        }
        break;
      case fullcircle::Snip::ABORT:
        {
          std::cout << "Abort!" << std::endl;
          fullcircle::Snip_AbortSnip abort = snip.abort_snip();
          _on_abort(abort);
          _active = false;
        }
        break;
      case fullcircle::Snip::EOS:
        {
          std::cout << "EOS!" << std::endl;
          fullcircle::Snip_EosSnip eos = snip.eos_snip();
          _on_eos(eos);
          _active = false;
        }
        break;
      default: 
        std::cout << "Unknown snip, discarding." << std::endl;
        break;
    }
  }

}

void ServerProtocolDispatcher::handle_error(
    boost::system::error_code error)
{
  std::cout << "ServerProtocolDispatcher::handle_error: " << error.message() << std::endl;
  _active = false;
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

boost::signals2::connection ServerProtocolDispatcher::do_on_timeout_snip(
    const on_timeout_snip_slot_t& slot)
{
  return _on_timeout_snip.connect(slot);
}

boost::signals2::connection ServerProtocolDispatcher::do_on_abort(
    const on_abort_snip_slot_t& slot)
{
  return _on_abort.connect(slot);
}

boost::signals2::connection ServerProtocolDispatcher::do_on_eos(
    const on_eos_snip_slot_t& slot)
{
  return _on_eos.connect(slot);
}

boost::signals2::connection ServerProtocolDispatcher::do_on_timeout(
    const on_timeout_slot_t& slot)
{
  return _on_timeout.connect(slot);
}

void ServerProtocolDispatcher::timeout(const boost::system::error_code& error)
{
  if ( error == boost::asio::error::operation_aborted ) {
    std::cout << "Timer aborted!" << std::endl;
  } else if ( !error )
  {
    std::cout << "Timeout!" << std::endl;
    _on_timeout();
    send_timeout();
  }
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
  _timer.expires_from_now(boost::posix_time::seconds(5));
  _timer.async_wait(boost::bind(&ServerProtocolDispatcher::timeout, this, _1));
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

void ServerProtocolDispatcher::send_timeout() {
  fullcircle::Snip snip;
  snip.set_type(fullcircle::Snip::TIMEOUT);
  std::ostringstream oss2;
  if (!snip.SerializeToOstream(&oss2)) {
    std::cout << "start snip serialization did not work." << std::endl;
  }
  oss2.flush();
  fullcircle::Envelope::Ptr renv(new fullcircle::Envelope());
  renv->set_body(oss2.str());
  _transport->write(renv);
}

bool ServerProtocolDispatcher::is_active()
{
  return _active;
}

void ServerProtocolDispatcher::setSession(ServerSession::Ptr session)
{
  _session = session;
}

ServerSession::Ptr ServerProtocolDispatcher::getSession()
{
  return _session;
}
