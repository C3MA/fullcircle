#ifndef LIBFULLCIRCLE_FONTRENDERER_HPP
#define LIBFULLCIRCLE_FONTRENDERER_HPP 1

#include <ostream>
#include <libfullcircle/common.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/frame.hpp>
#include <string.h>

namespace fullcircle {

	static const RGB_t COLOR_SET = { 255, 255, 255 };
	static const RGB_t COLOR_TRANSPARENT = { 0, 0, 0 };
	
  class FontRenderer {
    public:
      typedef std::tr1::shared_ptr<FontRenderer> Ptr;

      FontRenderer ( uint16_t width, uint16_t height);
      FontRenderer (Frame& rhs);
      virtual ~FontRenderer() {};
      void setSize(uint16_t font_width, uint16_t font_height);

      void write_text(Sequence::Ptr sequence, uint16_t x_offset, uint16_t y_offset, std::string text);
      void write_text(Sequence::Ptr sequence, uint16_t x_offset, uint16_t y_offset, std::string text, RGB_t textColor);
      void scroll_text(Sequence::Ptr sequence, uint16_t x_offset, uint16_t y_offset, std::string text, uint16_t	scrollspeed_inms);
      void scroll_text(Sequence::Ptr sequence, uint16_t x_offset, uint16_t y_offset, std::string text, uint16_t	scrollspeed_inms, RGB_t textColor);


    private:
      Frame::Ptr searchCharacter(char c);
      uint16_t _width;
      uint16_t _height;
      uint16_t _font_width;
      uint16_t _font_height;
  };
};


#endif /* LIBFULLCIRCLE_FRAME_HPP */

