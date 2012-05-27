#include "sequence.hpp"

using namespace fullcircle;

Sequence::Sequence ( const uint8_t& frames_per_second,
    const uint8_t& x_dim, const uint8_t& y_dim)
  : _fps(frames_per_second)
  , _x_dim(x_dim)
  , _y_dim(y_dim)
  , _frames()
{
}

void Sequence::add_frame(Frame::Ptr frame) {
  if (frame->x_dim() != x_dim() || 
      frame->y_dim() != y_dim())
    throw DataFormatException("Pixel dimension mismatch - cannot add frame.");
  _frames.push_back(frame);
}

void Sequence::dump(std::ostream& os) {
  uint32_t seq_number = 0;
  std::vector<Frame::Ptr>::iterator it;
  for(  it = _frames.begin(); it != _frames.end(); it++) {
    os << "### " << seq_number++ << " #################################" << std::endl;
    (*it)->dump_frame(os);
  }
  os << "#######################################" << std::endl;
}
