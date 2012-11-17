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
    std::cout << "Reconstructed snip: " << snip.DebugString() << std::endl;
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
      case fullcircle::Snip::ACK:
        {
          fullcircle::Snip_AckSnip ack=snip.ack_snip();
          _on_ack(ack);
        }
        break;
      case fullcircle::Snip::NACK:
        {
          fullcircle::Snip_NackSnip nack=snip.nack_snip();
          _on_nack(nack);
        }
        break;
      case fullcircle::Snip::START:
        {
          fullcircle::Snip_StartSnip start = snip.start_snip();
          _on_start(start);
        }
        break;
      default: 
        std::cout << "Unknown snip (" << snip.type() << "), discarding." << std::endl;
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

void ClientDispatcher::send_request(std::string color, uint32_t seq_id, fullcircle::BinarySequenceMetadata* meta) {
  fullcircle::Snip snip;
  std::cout << "Ask the server to get a timeslot..." << std::endl;
  snip.set_type(fullcircle::Snip::REQUEST);
  fullcircle::Snip_RequestSnip* request = snip.mutable_req_snip();
  request->set_color(color);
  request->set_seqid(seq_id);
  //copy metadata to request
  fullcircle::BinarySequenceMetadata* metadata = request->mutable_meta();
  metadata->set_frames_per_second(meta->frames_per_second());
  metadata->set_width(meta->width());
  metadata->set_height(meta->height());
  metadata->set_generator_name(meta->generator_name());
  metadata->set_generator_version(meta->generator_version());
  std::ostringstream oss;
  if ( !snip.SerializeToOstream(&oss)) {
    std::cerr << "request snip serialization did not work." << std::endl;
  }
  oss.flush();
  fullcircle::Envelope::Ptr env(new fullcircle::Envelope());
  env->set_body(oss.str());
  std::cout << "Preparation complete" << std::endl;
  _transport->write(env);
  std::cout << "sent!!!!" << std::endl;
}

void ClientDispatcher::send_frame(fullcircle::Frame::Ptr frame) {
  fullcircle::Snip snip;
  snip.set_type(fullcircle::Snip::FRAME);
  fullcircle::Snip_FrameSnip* framesnip = snip.mutable_frame_snip();
  fullcircle::BinaryFrame* bframe = framesnip->mutable_frame();
  for (uint16_t x=0; x<frame->width(); ++x) {
    for (uint16_t y=0; y<frame->height(); ++y) {
      fullcircle::RGB_t pixel=frame->get_pixel(x,y);
      fullcircle::RGB_Value* binpixel = bframe->add_pixel();
      binpixel->set_red(pixel.red);
      binpixel->set_green(pixel.green);
      binpixel->set_blue(pixel.blue);
      binpixel->set_x(x);
      binpixel->set_y(y);
    }
  }
  std::ostringstream oss;
  if ( !snip.SerializeToOstream(&oss)) {
    std::cerr << "request snip serialization did not work." << std::endl;
  }
  oss.flush();
  fullcircle::Envelope::Ptr env(new fullcircle::Envelope());
  env->set_body(oss.str());
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

boost::signals2::connection ClientDispatcher::do_on_ack(
    const on_ack_snip_slot_t& slot) 
{
  return _on_ack.connect(slot);
}

boost::signals2::connection ClientDispatcher::do_on_nack(
    const on_nack_snip_slot_t& slot) 
{
  return _on_nack.connect(slot);
}

boost::signals2::connection ClientDispatcher::do_on_start(
    const on_start_snip_slot_t& slot) 
{
  return _on_start.connect(slot);
}


