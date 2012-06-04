#ifndef LIBFULLCIRCLE_FONTRENDERER_HPP
#define LIBFULLCIRCLE_FONTRENDERER_HPP 1

#include <libfullcircle/common.hpp>
#include <ostream>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/frame.hpp>
#include <map>

namespace fullcircle {

	static const RGB_t COLOR_SET = { 0, 0, 0 };
	static const RGB_t COLOR_TRANSPARENT = { 255, 255, 255 };
	
  class FontRenderer {
    public:
      typedef std::tr1::shared_ptr<FontRenderer> Ptr;

	  typedef std::map<uint8_t, Frame::Ptr> ascii_map_t;
	  
      FontRenderer ( uint16_t width, uint16_t height);
      FontRenderer (Frame& rhs);
      virtual ~FontRenderer() {};
	  
	  /*
	   * The data is generated and stored in a file
	   * http://www.pentacom.jp/pentacom/bitfontmaker2/
	   */
	  void load_font(std::string font_file);
	  
	  void write_text(Sequence::Ptr sequence, uint16_t x, uint16_t y, std::string text);
	  
    private:
      uint16_t _width;
      uint16_t _height;
	  ascii_map_t _asciiMapping;
  };
};


#endif /* LIBFULLCIRCLE_FRAME_HPP */

