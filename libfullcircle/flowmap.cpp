#include "flowmap.hpp"
#include <cstdlib>
#include <iostream>
#include <cmath>


using namespace fullcircle;


FlowMap::FlowMap()
{
	
}

void FlowMap::init(std::string hash, uint16_t width, uint16_t height)
{
		//FIXME this has to be done later...
}

void FlowMap::init(Frame::Ptr hills)
{
	// make a deep copy
	fullcircle::Frame::Ptr frame(new fullcircle::Frame(hills->width(), hills->height()));
	_hills = frame;
	_hills->set_pixel(0,0, hills);
}

Frame::Ptr FlowMap::get_next()
{
	//FIXME big TODO.
	
	// let the water flow!
	
	return _actualColoredFrame;
}


bool FlowMap::has_changed()
{
	return (_oldColoredFrame != _actualColoredFrame);
}