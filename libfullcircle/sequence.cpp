#include "sequence.hpp"
#include <libfullcircle/sequence.pb.h>
#include <boost/multi_array.hpp>
#include <sstream>

#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <exception>
#include <fstream>

#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

namespace pod = boost::program_options::detail;

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

///**
// * An automatic sequence constructor.
// * When no configuration is found, a default configuration is used.
// ****************/
//Sequence::Sequence()
//: _fps()
//, _width()
//, _height()
//, _generator_name()
//, _generator_version()
//, _frames()
//{
//	char * pHome;
//	pHome = getenv ("HOME");		
//	
//	char* cfgPath = strcat(pHome, CONFIGURATION_PATH);
//	std::ifstream config(cfgPath);
//	
//    if(!config)
//    {
//		// set the default values
//        _fps = DEFAULT_FPS;
//		_height = DEFAULT_HEIGHT;
//		_width = DEFAULT_WIDTH;
//		std::cerr << "FAILED, we use the standard configuration" << std::endl;
//    }
//	else
//	{
//		//parameters
//		std::set<std::string> options;
//		std::map<std::string, std::string> parameters;
//		options.insert("*");
//		
//		try
//		{  
//			// fill all the values from the configuration file into the structure
//			for (pod::config_file_iterator i(config, options), e ; i != e; ++i)
//			{
//				//std::cout << i->string_key <<" "<<i->value[0] << std::endl; //Debug line
//				parameters[i->string_key] = i->value[0];
//			}
//			// first reset all attributes
//			_fps = 0;
//			_height = 0;
//			_width = 0;
//			// now try to fill the attributes with the configuration.
//			_width = atoi( parameters["width"].c_str() );
//			_height = atoi( parameters["height"].c_str() );
//			_fps = atoi( parameters["fps"].c_str() );
//		}
//		catch(std::exception& e)    
//		{
//			std::cerr<<"Exception: "<<e.what()<<std::endl;
//		}
//	}
//}

void Sequence::add_frame(Frame::Ptr frame) {
  if (frame->width() != width() || 
      frame->height() != height())
    throw DataFormatException("Pixel dimension mismatch - cannot add frame.");
  _frames.push_back(frame);
}

void Sequence::add_sequence(Sequence::Ptr rhs)
{
	for (uint32_t frameID=0; frameID < rhs->size(); ++frameID) {
		add_frame(rhs->get_frame(frameID));
	}
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

Sequence::Ptr Sequence::add(uint32_t frame_offset, Sequence::Ptr rhs, bool ringBuffer)
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
		} else if (ringBuffer) {
			/* the first sequence is too short, start with the first frame of this sequence */
			fullcircle::Frame::Ptr local = get_frame( frameID % size() );
			fullcircle::Frame::Ptr sum = (*local) + other;
			retval->add_frame(sum);
		} else {
			/* the first sequence is too short, so simply append the second sequence */
			retval->add_frame(other);
		}
	}
	
	// add the possible rest of local frames
	for (uint32_t frameID=rhs->size(); frameID < size(); ++frameID) {
		if (ringBuffer) {
			/* the second sequence is too short, start with the first frame of this sequence */
			fullcircle::Frame::Ptr other = rhs->get_frame( frameID % rhs->size() );
			fullcircle::Frame::Ptr local = get_frame( frameID );
			fullcircle::Frame::Ptr sum = (*local) + other;
			retval->add_frame(sum);
		} else {
			fullcircle::Frame::Ptr local = get_frame(frameID);
			retval->add_frame(local);
		}
	}
	
	return retval;
}

Sequence::Ptr Sequence::operator+(Sequence::Ptr rhs) {
  return add(0, rhs);
}
