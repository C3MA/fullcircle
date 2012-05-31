#include "frame.hpp"
#include <iostream>


using namespace fullcircle;

Frame::Frame ( uint16_t width, uint16_t height)
  : _width(width)
  , _height(height)
  , _pixels(boost::extents[_width][_height])
{
    RGB_t black;
    black.red = black.green = black.blue = 0;
    for (uint16_t x=0; x<width; x++) {
      for (uint16_t y=0; y<height; y++) {
        set_pixel(x, y, black); 
      }
    }
};

Frame::Frame (Frame& rhs) 
  : _width(rhs.width())
  , _height(rhs.height())
  , _pixels(boost::extents[_width][_height])
{
    for (uint16_t x=0; x<_width; ++x) {
      for (uint16_t y=0; y<_height; ++y) {
        set_pixel(x, y, rhs.get_pixel(x,y)); 
      }
    }
};

inline void Frame::check_coordinates (
    const uint16_t& x,
    const uint16_t& y ) 
{
  if ((x >= _width) || (y >= _height) ) 
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
  for (uint16_t y=0; y < _height; ++y) {
    for (uint16_t x=0; x < _width; ++x) {
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
  if (width() != rhs.width() ||
      height() != rhs.height())
    return false;
  for (uint16_t x=0; x < _width; ++x) {
    for (uint16_t y=0; y < _height; ++y) {
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
