#include "frame.hpp"
#include <iostream>


using namespace fullcircle;

Frame::Frame ( uint16_t x_dim, uint16_t y_dim)
  : _x_dim(x_dim)
  , _y_dim(y_dim)
  , _pixels(boost::extents[_x_dim][_y_dim])
{
    RGB_t black;
    black.red = black.green = black.blue = 0;
    for (uint16_t x=0; x<x_dim; x++) {
      for (uint16_t y=0; y<y_dim; y++) {
        set_pixel(x, y, black); 
      }
    }
};

Frame::Frame (Frame& rhs) 
  : _x_dim(rhs.x_dim())
  , _y_dim(rhs.y_dim())
  , _pixels(boost::extents[_x_dim][_y_dim])
{
    for (uint16_t x=0; x<_x_dim; ++x) {
      for (uint16_t y=0; y<_y_dim; ++y) {
        set_pixel(x, y, rhs.get_pixel(x,y)); 
      }
    }
};

inline void Frame::check_coordinates (
    const uint16_t& x,
    const uint16_t& y ) 
{
  if ((x >= _x_dim) || (y >= _y_dim) ) 
    // Not needed: || (x < 0) || (y < 0)) uint16_t cannot be negative
    throw DataFormatException("Pixel coordinates out of bound");
}

void Frame::set_pixel(
    const uint16_t& x,
    const uint16_t& y,
    const uint16_t& red,
    const uint16_t& green,
    const uint16_t& blue
) {
  RGB_t color;
  color.red=red;
  color.green=green;
  color.blue=blue;
  set_pixel(x, y, color);
}

void Frame::set_pixel(
    const uint16_t& x,
    const uint16_t& y,
    const RGB_t& color
) {
  check_coordinates(x,y);
  _pixels[x][y]=color;
}

const RGB_t Frame::get_pixel(
    const uint16_t& x,
    const uint16_t& y
  ) {
  check_coordinates(x,y);
  return _pixels[x][y];
}


void Frame::dump_frame(std::ostream& os) {
  for (uint16_t y=0; y < _y_dim; ++y) {
    for (uint16_t x=0; x < _x_dim; ++x) {
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


bool Frame::operator== (Frame &rhs) {
  if (x_dim() != rhs.x_dim() ||
      y_dim() != rhs.y_dim())
    return false;
  for (uint16_t x=0; x < _x_dim; ++x) {
    for (uint16_t y=0; y < _y_dim; ++y) {
      RGB_t thisc=get_pixel(x,y);
      RGB_t rhsc=rhs.get_pixel(x,y);
      if (thisc.red != rhsc.red ||
          thisc.green != rhsc.green ||
          thisc.blue != rhsc.blue)
        return false;
    }
  }
  return true;
}


bool Frame::operator!= (Frame &rhs) {
  return !(*this == rhs);
}
