#include "sequence.hpp"
#include <libfullcircle/sequence.pb.h>
#include <iostream>

using namespace fullcircle;

Sequence::Sequence ( const uint8_t& frames_per_second,
    const uint8_t& x_dim, const uint8_t& y_dim)
  : _fps(frames_per_second)
  , _x_dim(x_dim)
  , _y_dim(y_dim)
  , _generator_name()
  , _generator_version()
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

void Sequence::save(std::ostream& os, 
    const std::string& generator,
    const std::string& version
    ) {
  fullcircle::BinarySequence bseq;
  // Handle Metadata
  fullcircle::BinarySequenceMetadata* metadata=bseq.mutable_metadata();
  metadata->set_frames_per_second(_fps);
  metadata->set_x_dim(x_dim());
  metadata->set_y_dim(y_dim());
  metadata->set_generator_name(generator);
  metadata->set_generator_version(version);
  // Handle ALL teh frames
  fullcircle::Sequence::const_iterator it;
  for (it = begin(); it != end(); ++it) {
    fullcircle::BinaryFrame* binframe=bseq.add_frame();
    for (uint8_t x=0; x<_x_dim; ++x) {
      for (uint8_t y=0; y<_y_dim; ++y) {
        RGB_t pixel=(*it)->get_pixel(x,y);
        fullcircle::RGB_Value* binpixel=binframe->add_pixel();
        binpixel->set_red(pixel.red);
        binpixel->set_green(pixel.green);
        binpixel->set_blue(pixel.blue);
        binpixel->set_x(x);
        binpixel->set_y(y);
      }
    }
  }
  if (!bseq.SerializeToOstream(&os)) {
    throw fullcircle::StoreException("Cannot serialize data while storing.");
  }
  os.flush();
  //std::cout << "Debug: " << bseq.DebugString() << std::endl;
}

Sequence::Sequence(std::istream& is) 
  : _frames()
{
  fullcircle::BinarySequence bseq;
  if (!bseq.ParseFromIstream(&is)) {
    throw fullcircle::StoreException("Cannot load data from input stream.");
  }
  //std::cout << "Debug: " << bseq.DebugString() << std::endl;
  fullcircle::BinarySequenceMetadata metadata=bseq.metadata();
  _fps=metadata.frames_per_second();
  _x_dim=metadata.x_dim();
  _y_dim=metadata.y_dim();
  _generator_name=metadata.generator_name();
  _generator_version=metadata.generator_version();
  for( int frameID = 0; frameID < bseq.frame_size(); frameID++) {
    fullcircle::Frame::Ptr frame(new fullcircle::Frame(_x_dim, _y_dim));
    fullcircle::BinaryFrame binframe=bseq.frame(frameID);
    for( int pixelID = 0; pixelID < binframe.pixel_size(); pixelID++) {
      fullcircle::RGB_Value binpixel=binframe.pixel(pixelID);
      frame->set_pixel(binpixel.x(), binpixel.y(), 
          binpixel.red(), binpixel.green(), binpixel.blue());
    }
    _frames.push_back(frame);
  }
}


Frame::Ptr Sequence::get_frame(uint32_t frameid) {
  if (frameid < size())
    return _frames[frameid];
  else 
    throw fullcircle::IndexOutOfBoundException("Invalid index during get_frame");
}

bool Sequence::operator== (Sequence &rhs) {
  if (x_dim() != rhs.x_dim() ||
      y_dim() != rhs.y_dim() ||
      fps() != rhs.fps() ||
      size() != rhs.size()
      ) {
    return false;
  }
  try {
    for (uint8_t frameID=0; frameID < size(); ++frameID) {
      fullcircle::Frame::Ptr lhs_frame = get_frame(frameID);
      fullcircle::Frame::Ptr rhs_frame = rhs.get_frame(frameID);
      if (*lhs_frame != *rhs_frame) {
        return false;
      }
    }
  } catch (fullcircle::IndexOutOfBoundException const& ex) {
    return false;
  }
  return true;
}

bool Sequence::operator!= (Sequence &rhs) {
  return !(*this == rhs);
}
