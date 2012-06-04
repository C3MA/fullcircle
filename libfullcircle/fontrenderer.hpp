#ifndef LIBFULLCIRCLE_FONTRENDERER_HPP
#define LIBFULLCIRCLE_FONTRENDERER_HPP 1

#include <libfullcircle/common.hpp>
#include <ostream>
#include "sequence.hpp"

namespace fullcircle {

  class FontRenderer {
    public:
      typedef std::tr1::shared_ptr<FontRenderer> Ptr;

      FontRenderer ( uint16_t width, uint16_t height);
      FontRenderer (Frame& rhs);
      virtual ~FontRenderer() {};
	  void load_font(std::string font_file);
	  void write_text(Sequence::Ptr sequence, uint16_t x, uint16_t y, std::string text);
	  
    private:
      uint16_t _width;
      uint16_t _height;
  };
};


#endif /* LIBFULLCIRCLE_FRAME_HPP */

