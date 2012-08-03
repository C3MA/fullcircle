#include "flowmap.hpp"
#include <cstdlib>
#include <iostream>
#include <cmath>


using namespace fullcircle;


FlowMap::FlowMap()
{
	_flowspeed = 1;
}

void FlowMap::init(std::string hash, uint16_t width, uint16_t height)
{
		//FIXME this has to be done later...
}

/**
 * @param[in] hills 
 *		This Frame describes the height of each pixel. The color on each pixel describes the height.
 *		red, green and blue are simply added -> the values could be between 0 and 768.
 *
 */
void FlowMap::init(Frame::Ptr hills)
{
	// make a deep copy
	fullcircle::Frame::Ptr frame(new fullcircle::Frame(hills->width(), hills->height()));
	_hills = frame;
	_hills->set_pixel(0,0, hills);
	
	// initialize the frame for the last colored step
	fullcircle::Frame::Ptr oldframe(new fullcircle::Frame(hills->width(), hills->height()));
	_oldColoredFrame = oldframe;
}

void FlowMap::start_points(Frame::Ptr start)
{
	if (start->width() != _hills->width())
		throw fullcircle::RenderException("Screen has not the same width");
	if (start->height() != _hills->height())
		throw fullcircle::RenderException("Screen has not the same height");
	
	// make a deep copy
	fullcircle::Frame::Ptr frame(new fullcircle::Frame(start->width(), start->height()));
	_actualColoredFrame = frame;
	_actualColoredFrame->set_pixel(0,0, start);	
}

/**
 * Calculate a height in the map from a given color.
 * @param[in] frame
 *		The frame, the height should be calculated from.
 * @param[in] x
 *		The offset in horizontal direction (could be out of bounds, than the height will always be at the maximum)
 * @param[in] y
 *		The offset from top to down (could be out of bounds, than the height will always be at the maximum)
 */
uint32_t FlowMap::calc_height(Frame::Ptr frame, int32_t x, int32_t y)
{
	if (x < 0 || x >= frame->width() || y < 0 || y >= frame->height())
		return MAXIMUM;
	RGB_t heightColor = frame->get_pixel(x,y);
	return heightColor.red + heightColor.green + heightColor.blue;
}

Frame::Ptr FlowMap::get_next()
{
	// make a deep copy of the 
	_oldColoredFrame->set_pixel(0,0, _actualColoredFrame);	
	
	int32_t diff[8];
	bool removeColor = false;
	
	for (uint16_t x=0; x < _actualColoredFrame->width(); x++) {
		for (uint16_t y=0; y < _actualColoredFrame->height(); y++) {
			uint32_t actHeight = calc_height(_hills, x, y);
			// previous line
			diff[0] = calc_height(_hills, x - 1, y - 1) - actHeight;
			diff[1] = calc_height(_hills, x, y - 1) - actHeight;
			diff[2] = calc_height(_hills, x + 1, y - 1) - actHeight;
			// same line, like the actual position
			diff[3] = calc_height(_hills, x - 1, y) - actHeight;
			diff[4] = calc_height(_hills, x + 1, y) - actHeight;
			// following line
			diff[5] = calc_height(_hills, x - 1, y + 1) - actHeight;
			diff[6] = calc_height(_hills, x, y + 1) - actHeight;
			diff[7] = calc_height(_hills, x + 1, y + 1) - actHeight;
			
			for (int i=0; i < 8; i++) {
				if (diff[i] < 0)
					removeColor = true;
			}
			
			// There was a sink found, so the color at the current pixel has to be removed.
			if (removeColor)
			{
				RGB_t actualColor = _actualColoredFrame->get_pixel(x,y);
				if (actualColor.red > 0)
					actualColor.red -= _flowspeed;
				if (actualColor.green > 0)
					actualColor.green -= _flowspeed;
				if (actualColor.blue > 0)
					actualColor.blue -= _flowspeed;
				_actualColoredFrame->set_pixel(x, y, actualColor);
			}
		}
	}
	
	// let the water flow!
	
	
	
	return _actualColoredFrame;
}


void FlowMap::set_speed(uint16_t flowspeed)
{
	_flowspeed = flowspeed;
}

bool FlowMap::has_changed()
{
	return (_oldColoredFrame != _actualColoredFrame);
}