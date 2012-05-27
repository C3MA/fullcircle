#include "frame.hpp"

using namespace fullcircle;

Frame::Frame ( const uint8_t& x_dim, const uint8_t& y_dim)
  : _x_dim(x_dim)
  , _y_dim(y_dim)
  , _framedata(x_dim * y_dim) {
    RGB_t black;
    black.red = black.green = black.blue = 0;
    for (uint8_t x=0; x<x_dim; ++x) {
      for (uint8_t y=0; y<y_dim; ++y) {
        set_pixel(x, y, black); 
      }
    }
};

inline void Frame::check_coordinates (
    const uint8_t& x,
    const uint8_t& y ) 
{
  if ((x > _x_dim) || (y > _y_dim) ) 
    // Not needed: || (x < 0) || (y < 0)) uint8_t cannot be negative
    throw DataFormatException("Pixel coordinates out of bound");
}

void Frame::set_pixel(
    const uint8_t& x,
    const uint8_t& y,
    const uint8_t& red,
    const uint8_t& green,
    const uint8_t& blue
) {
  RGB_t color;
  color.red=red;
  color.green=green;
  color.blue=blue;
  set_pixel(x, y, color);
}

void Frame::set_pixel(
    const uint8_t& x,
    const uint8_t& y,
    const RGB_t& color
) {
  check_coordinates(x,y);
  _framedata[(x*_x_dim)+y]=color;
}

const RGB_t Frame::get_pixel(
    const uint8_t& x,
    const uint8_t& y
  ) {
  check_coordinates(x,y);
  return _framedata[(x*_x_dim)+y];
}


void Frame::dump_frame(std::ostream& os) {
  for (uint8_t x=0; x < _x_dim; ++x) {
    for (uint8_t y=0; y < _y_dim; ++y) {
      RGB_t color=get_pixel(x,y);
      os.setf ( std::ios::hex, std::ios::basefield ); 
      os << "(" << (int)color.red << "," 
        << (int)color.green << "," 
        << (int)color.blue << ") ";
      os.unsetf( std::ios::hex);
      os.unsetf(std::ios::basefield);
    }
    os << std::endl;
  }
}
