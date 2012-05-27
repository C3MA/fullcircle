#include "frame.hpp"

using namespace fullcircle;

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
  _framedata[x*y]=color;
}

const RGB_t Frame::get_pixel(
    const uint8_t& x,
    const uint8_t& y
  ) {
  check_coordinates(x,y);
  return _framedata[x*y];
}


