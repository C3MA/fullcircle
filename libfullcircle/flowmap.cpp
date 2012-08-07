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
	// create a new frame, where the hills are written to
	fullcircle::Frame::Ptr frame(new fullcircle::Frame(width, height));
	
	_hills = frame;
	
	// Fill the frame with values.
	uint64_t arraySize = width * height;
	
	/* combine more and more characters together */
	uint32_t step;
	uint16_t combinedChar = 0;
	uint64_t tmp;
	do {
		combinedChar++;
		step = arraySize / (hash.length() / combinedChar);
	} while (step == 0);
	
    for( uint16_t i = 0; i < hash.length() && (i / width) < height; i += step) {
		tmp = hash[i];
		for (uint16_t j = 1 /* one is added before the loop*/; j < combinedChar; j++) {
			tmp += hash[i+j];
		}
		_hills->set_pixel(i / width, i % width, tmp & 0xFF0000, tmp & 0xFF00, tmp & 0xFF);
    }
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
 *		The offset in horizontal direction (could be out of bounds, than the height will always be at the minimum)
 * @param[in] y
 *		The offset from top to down (could be out of bounds, than the height will always be at the minimum)
 */
uint32_t FlowMap::calc_height(Frame::Ptr frame, int32_t x, int32_t y)
{
	if (x < 0 || x >= frame->width() || y < 0 || y >= frame->height())
		return 0;
	RGB_t heightColor = frame->get_pixel(x,y);
	return heightColor.red + heightColor.green + heightColor.blue;
}


void FlowMap::modify_pixel(uint16_t x, uint16_t y, int32_t diff, int32_t maxDiff, RGB_t actualColor)
{	
	// Only do something if the pixel we look at is deeper that our source
	if (diff >= 0)
		return;
	
	/*** swap the direction ***/
	diff = abs(diff);
	maxDiff = abs(maxDiff);
	
	RGB_t above = _actualColoredFrame->get_pixel(x, y);
	//std::cerr << "---- Diff (" << x << "x" << y << ") diff=" << diff << " max=" << maxDiff << std::endl;
	//std::cerr << "Old RGB " << above.red << "," << above.green << ", " << above.blue << std::endl;
	above.red += (actualColor.red * _flowspeed * diff) / maxDiff / NEIGHBOUR_FACTOR;
	above.green += (actualColor.green * _flowspeed * diff) / maxDiff / NEIGHBOUR_FACTOR;
	above.blue += (actualColor.blue * _flowspeed * diff) / maxDiff / NEIGHBOUR_FACTOR;
	_actualColoredFrame->set_pixel(x, y, above);
	//std::cerr << "New RGB " << above.red << "," << above.green << ", " << above.blue << std::endl;
}


Frame::Ptr FlowMap::get_next()
{
	// make a deep copy of the 
	_oldColoredFrame->set_pixel(0,0, _actualColoredFrame);	
	
	int32_t diff[8];
	int32_t maxDiff;

	for (uint16_t x=0; x < _actualColoredFrame->width(); x++)
	{
		for (uint16_t y=0; y < _actualColoredFrame->height(); y++) 
		{
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
			maxDiff = 0;
			for (int i=0; i < 8; i++) {
				// search the biggest negative diff
				if (maxDiff > diff[i])
					maxDiff = diff[i];
			}
			
			RGB_t actualColor = _oldColoredFrame->get_pixel(x,y);
			
			/*std::cerr << "===== RGB(" << x << "x" << y << ") " 
				<< actualColor.red << "," << actualColor.green << ", " << actualColor.blue 
				<< "\tmaxDiff= " << maxDiff << std::endl;*/
			
			// There was a sink found, so the color at the current pixel has to be removed.
			if (maxDiff < 0)
			{
				// first let the water flow to the neighbour pixel (according to the height difference)
				if (y > 0) // there is a row above
				{
					// modify the pixel direct above
					modify_pixel(x, y - 1, diff[1], maxDiff, actualColor);
					
					if (x > 0) // modify the pixel in the upper left
					{
						modify_pixel(x - 1, y - 1, diff[0], maxDiff, actualColor);
					}
					
					if (x < _actualColoredFrame->width() - 1) // modify the pixel in the upper right
					{
						modify_pixel(x + 1, y - 1, diff[2], maxDiff, actualColor);
					}					
				}
				
				if (x > 0) // modify the left pixel (same line)
				{
					modify_pixel(x - 1, y, diff[3], maxDiff, actualColor);
				}
				
				if (x < _actualColoredFrame->width() - 1) // modify the pixel up-right
				{
					modify_pixel(x + 1, y, diff[4], maxDiff, actualColor);
				}
				
				if (y < _actualColoredFrame->height() - 1) // there is a row below
				{
					// modify the pixel direct above
					modify_pixel(x, y + 1, diff[6], maxDiff, actualColor);
					
					if (x > 0) // modify the pixel in the upper left
					{
						modify_pixel(x - 1, y + 1, diff[5], maxDiff, actualColor);
					}
					
					if (x < _actualColoredFrame->width() - 1) // modify the pixel in the upper right
					{
						modify_pixel(x + 1, y + 1, diff[7], maxDiff, actualColor);
					}	
				}
				
				// decrement the color at the current pixel.
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

void FlowMap::dump_hills(std::ostream& os)
{
	_hills->dump_frame(os);
}
