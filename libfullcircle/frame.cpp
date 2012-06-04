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

void Frame::set_pixel(
			   const uint16_t& x_start,
			   const uint16_t& y_start,
			   Frame::Ptr mini_frame
			   )
{
	if (x_start + mini_frame->width() > _width)
		throw fullcircle::RenderException("Screen is not wide enough");
	if (y_start + mini_frame->height() > _height)
		throw fullcircle::RenderException("Screen is not high enough");
	
	for (uint16_t x=0; x<mini_frame->width(); ++x) {
		for (uint16_t y=0; y<mini_frame->height(); ++y) {
			set_pixel(x_start + x, y_start + y, mini_frame->get_pixel(x,y)); 
		}
    }
}

void Frame::set_pixel_window(
					  const uint16_t& x_start,
					  const uint16_t& y_start,
					  Frame::Ptr gigantic_frame
					  )
{
	if (x_start + _width > gigantic_frame->width())
		throw fullcircle::RenderException("gigantic screen is mini in the width");
	if (y_start + _height >  gigantic_frame->height())
		throw fullcircle::RenderException("gigantic screen is mini in the height");
	
	for (uint16_t x=0; x<_width; ++x) {
		for (uint16_t y=0; y<_height; ++y) {
			set_pixel(x, y, gigantic_frame->get_pixel(x_start + x, y_start + y)); 
		}
    }		
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

void Frame::swap_color(const RGB_t& from, const RGB_t& to)
{
	for (uint16_t x=0; x < _width; ++x) {
		for (uint16_t y=0; y < _height; ++y) {
			RGB_t thisc=get_pixel(x,y);
			if (thisc.red == from.red && thisc.green == from.green && thisc.blue == from.blue) {
				set_pixel(x, y, to);
			}
		}
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

void Frame::fill_whole(const RGB_t& color) {
	for (uint16_t x=0; x < _width; ++x) {
		for (uint16_t y=0; y < _height; ++y) {
			_pixels[x][y] = color;
		}
	}
}

void Frame::fill_gradient_horizontal(const RGB_t& from, const RGB_t& to) {
	RGB_t color = from;
	int16_t diffBlue = to.blue - from.blue;
	int16_t diffGreen = to.green - from.green;
	int16_t diffRed = to.red - from.red;

	std::cerr << "Diff red=" << diffRed << " green=" << diffGreen << " blue=" << diffBlue << std::endl;
	
	int16_t stepRed = diffRed / (_width - 1);
	int16_t stepGreen = diffGreen / (_width -1);
	int16_t stepBlue = diffBlue / (_width -1);
	std::cerr << "Steps red=" << stepRed << " green=" << stepGreen << " blue=" << stepBlue << std::endl;
	for (uint16_t x=0; x < _width; ++x) {
		for (uint16_t y=0; y < _height; ++y) {
			_pixels[x][y] = color;
		}
		// Set color for the next column
		color.red = (color.red + stepRed) % 255;
		color.green = (color.green + stepGreen) % 255;
		color.blue = (color.blue + stepBlue) % 255;
		if (x == _width - 2) // when we reched the last, update the value to the color we want reach
			color = to;
	}	
}

void Frame::fill_gradient_vertical(const RGB_t& from, const RGB_t& to) {
	RGB_t color = from;
	int16_t diffBlue = to.blue - from.blue;
	int16_t diffGreen = to.green - from.green;
	int16_t diffRed = to.red - from.red;
	
	uint16_t stepRed = diffRed / _height-1;
	uint16_t stepGreen = diffGreen / _height - 1;
	uint16_t stepBlue = diffBlue / _height - 1;
	
	for (uint16_t y=0; y < _height; ++y) {
		for (uint16_t x=0; x < _width; ++x) {
			_pixels[x][y] = color;
		}
		// Set color for the next row
		color.red = (color.red + stepRed) % 255;
		color.green = (color.green + stepGreen) % 255;
		color.blue = (color.blue + stepBlue) % 255;
		if (y == _height - 2) // when we reched the last, update the value to the color we want reach
			color = to;
	}
}