#ifndef LIBFULLCIRCLE_SEQUENCE_HPP
#define LIBFULLCIRCLE_SEQUENCE_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/frame.hpp>
#include <istream>
#include <ostream>

namespace fullcircle {
  class Sequence {
    public:
      typedef boost::shared_ptr<Sequence> Ptr;
      typedef std::vector<Frame::Ptr>::const_iterator const_iterator;
      Sequence (
          const uint16_t& frames_per_second, 
          const uint16_t& width, 
          const uint16_t& height);
      Sequence (std::istream& is);
//	  Sequence();
      virtual ~Sequence() {}; 

      void add_frame(Frame::Ptr frame);
      void add_sequence(Sequence::Ptr rhs);
      void extract_sequence(Sequence::Ptr rhs, uint32_t frame_offset, uint32_t snippet_length);
      Frame::Ptr get_frame(uint32_t frameid);
      Frame::Ptr get_last_frame();
      uint16_t width() const { return _width; };
      uint16_t height() const { return _height; };
      int fps() const { return _fps; };
      const std::string& generator_name() const { return _generator_name; };
      const std::string& generator_version() const { return _generator_version; };
      uint32_t size() const { return _frames.size(); };
      void trim_end();
      void dump(std::ostream& os);
      Sequence::Ptr add(uint32_t frame_offset, Sequence::Ptr rhs, bool ringBuffer = false);
      void save(std::ostream& os,
          const std::string& generator,
          const std::string& version);
      const_iterator begin() const {
        return (_frames.begin()); 
      };
      const_iterator end() const {
        return (_frames.end()); 
      };

      bool operator== (Sequence &rhs);
      bool operator!= (Sequence &rhs);
      Sequence::Ptr operator<< (Sequence::Ptr rhs);
      Sequence::Ptr operator+(Sequence::Ptr rhs);

    private:
      Sequence (const Sequence& original);
      Sequence& operator= (const Sequence& rhs);
      uint16_t _fps;
      uint16_t _width;
      uint16_t _height;
      std::string _generator_name;
      std::string _generator_version;
      std::vector<Frame::Ptr> _frames;

	  // default values
	  const static uint16_t DEFAULT_FPS = 25;
	  const static uint16_t DEFAULT_WIDTH = 16;
	  const static uint16_t DEFAULT_HEIGHT = 9;
	  #define CONFIGURATION_PATH "/.libfullcircle_rc\0"
  };
};


#endif /* LIBFULLCIRCLE_SEQUENCE_HPP */

