#include "sequence.hpp"
#include <libfullcircle/sequence.pb.h>
#include <boost/multi_array.hpp>
#include <iostream>
#include <sstream>

using namespace fullcircle;

Sequence::Sequence ( const uint16_t& frames_per_second,
    const uint16_t& width, const uint16_t& height)
  : _fps(frames_per_second)
  , _width(width)
  , _height(height)
  , _generator_name()
  , _generator_version()
  , _frames()
{
}

void Sequence::add_frame(Frame::Ptr frame) {
  if (frame->width() != width() || 
      frame->height() != height())
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
  metadata->set_width(width());
  metadata->set_height(height());
  metadata->set_generator_name(generator);
  metadata->set_generator_version(version);
  // Handle ALL teh frames
  fullcircle::Sequence::const_iterator it;
  for (it = begin(); it != end(); ++it) {
    fullcircle::BinaryFrame* binframe=bseq.add_frame();
    for (uint16_t x=0; x<_width; ++x) {
      for (uint16_t y=0; y<_height; ++y) {
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
  _width=metadata.width();
  _height=metadata.height();
  _generator_name=metadata.generator_name();
  _generator_version=metadata.generator_version();
  for( int frameID = 0; frameID < bseq.frame_size(); frameID++) {
    fullcircle::Frame::Ptr frame(new fullcircle::Frame(_width, _height));
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
  else {
    std::ostringstream oss;
    oss << "get_frame: Invalid index " << frameid << " (size: " << size() << ")";
    throw fullcircle::IndexOutOfBoundException(oss.str());
  }
}

Frame::Ptr Sequence::get_last_frame() {
  return get_frame(size()-1);
}

bool Sequence::operator== (Sequence &rhs) {
  if (width() != rhs.width() ||
      height() != rhs.height() ||
      fps() != rhs.fps() ||
      size() != rhs.size()
      ) {
    return false;
  }
  try {
    for (uint32_t frameID=0; frameID < size(); ++frameID) {
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

void Sequence::trim_end()
{
	uint32_t length = size(); // use this as endmark (we have uint -> always > 0)
	for (uint32_t frameID=length - 1; frameID < length; frameID--) {
		fullcircle::Frame::Ptr item = get_frame(frameID);
		if (item->is_black())
		{
			_frames.erase(_frames.begin() + frameID);
		}
	}
}

bool Sequence::operator!= (Sequence &rhs) {
  return !(*this == rhs);
}


Sequence::Ptr Sequence::operator<< (Sequence::Ptr rhs) {
  if (rhs->fps() != fps())
    throw DataFormatException("Sequence FPS mismatch - cannot append frame.");
  Sequence::Ptr retval(new Sequence(
        _fps, 
        _width, 
        _height));
  fullcircle::Sequence::const_iterator it;
  for (it = this->begin(); it != this->end(); ++it) {
    retval->add_frame(*it);
  }
  for (it = rhs->begin(); it != rhs->end(); ++it) {
    retval->add_frame(*it);
  }
  return retval;
}

Sequence::Ptr Sequence::add(uint32_t frame_offset, Sequence::Ptr rhs)
{
	if (rhs->fps() != fps())
		throw DataFormatException("Sequence FPS mismatch - cannot add frame.");
	if (frame_offset > size())
		throw DataFormatException("The offset is larger than the size of this sequence");
	Sequence::Ptr retval(new Sequence(_fps, 
									  _width, 
									  _height));
	// add all the frames from the old sequence before the overlay logic starts
	for (uint32_t frameID=0; frameID < frame_offset; ++frameID) {
		fullcircle::Frame::Ptr local = get_frame(frameID);
		retval->add_frame(local);
	}
	
	// now create the overlay of the two sequences
	for (uint32_t frameID=frame_offset; (frameID - frame_offset) < rhs->size(); ++frameID) {
		fullcircle::Frame::Ptr other = rhs->get_frame(frameID - frame_offset);
		if (frameID < size()) {
			fullcircle::Frame::Ptr local = get_frame(frameID);
			fullcircle::Frame::Ptr sum = (*local) + other;
			retval->add_frame(sum);
		} else {
			retval->add_frame(other);
		}
	}
	// add the possible rest of local frames
	for (uint32_t frameID=rhs->size(); frameID < size(); ++frameID) {
		fullcircle::Frame::Ptr local = get_frame(frameID);
		retval->add_frame(local);
	}
	
	return retval;
}

void Sequence::append_fade(Frame::Ptr end, uint16_t steps) {
  if (size()==0) {
    std::ostringstream oss;
    oss << "append_fade: cannot fade on empty sequence (need at least" 
      << " one frame in the sequence as a start)";
    throw fullcircle::IndexOutOfBoundException(oss.str());
  }
  /**
   * This is a two-step algorithm. First, compute the velocity for each pixel (how
   * fast to fade the three color channels). Then, generate #steps frames
   * according to the precomputed velocity for each pixel and channel.
   */
  /**
   * Step1: Calculate the velocities. They are stored in a 3dim. array.
   * First dimension: X, second: Y, 3rd: R, G, B
   */
  typedef boost::multi_array<int, 3> velocity_array;
  typedef velocity_array::index index;
  velocity_array velocity(
      boost::extents[end->width()][end->height()][3]);
  uint8_t RED=0;
  uint8_t GREEN=1;
  uint8_t BLUE=2;
  fullcircle::Frame::Ptr last_frame=get_last_frame();
  for (uint16_t y=0; y < end->height(); ++y) {
    for (uint16_t x=0; x < end->width(); ++x) {
      RGB_t dest_color=end->get_pixel(x,y);
      RGB_t last_color=last_frame->get_pixel(x,y);
      velocity[x][y][RED]=(dest_color.red - last_color.red)/(float)steps;
      velocity[x][y][GREEN]=(dest_color.green - last_color.green)/(float)steps;
      velocity[x][y][BLUE]=(dest_color.blue - last_color.blue)/(float)steps;
    }
  }
  // Step2: Apply the velocities
  for( uint16_t s = 0; s < steps; ++s) {
    fullcircle::Frame::Ptr current_frame=
      fullcircle::Frame::Ptr(new fullcircle::Frame(end->width(), end->height()));
    for (uint16_t y=0; y < end->height(); ++y) {
      for (uint16_t x=0; x < end->width(); ++x) {
        RGB_t last_color=last_frame->get_pixel(x,y);
        RGB_t current_color;
        current_color.red=last_color.red + (steps * velocity[x][y][RED]);
        current_color.green=last_color.green + (steps * velocity[x][y][GREEN]);
        current_color.blue=last_color.blue + (steps * velocity[x][y][BLUE]);
        current_frame->set_pixel(x, y, current_color);
      }
    }
    add_frame(current_frame);
  }
  add_frame(end);
}

Sequence::Ptr Sequence::operator+(Sequence::Ptr rhs) {
  return add(0, rhs);
}
