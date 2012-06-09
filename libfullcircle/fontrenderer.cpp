#include "fontrenderer.hpp"
#include <QApplication>
#include <libfullcircle/sprite_io.hpp>
#include <iostream>

using namespace fullcircle;

FontRenderer::FontRenderer ( uint16_t width, uint16_t height)
  : _width(width)
  , _height(height)
{
	Q_INIT_RESOURCE(sprites);
	// Set the default font size of five times five
	_font_width = 5;
	_font_height = 5;
};

void FontRenderer::setSize(uint16_t font_width, uint16_t font_height)
{
	_font_width = font_width;
	_font_height = font_height;
}

void FontRenderer::write_text(Sequence::Ptr sequence, uint16_t x_offset, uint16_t y_offset, std::string text)
{
	scroll_text(sequence, x_offset, y_offset, text, 0);
}

void FontRenderer::write_text(Sequence::Ptr sequence, uint16_t x_offset, uint16_t y_offset, std::string text, RGB_t textColor)
{
	uint32_t startFrame=sequence->size();
	write_text(sequence, x_offset, y_offset, text);
	uint32_t length=sequence->size();
	for (; startFrame < length; startFrame++) {
		sequence->get_frame(startFrame)->swap_color(COLOR_SET, textColor);
	}
}


void FontRenderer::scroll_text(Sequence::Ptr sequence, uint16_t x_offset, uint16_t y_offset, 
							   std::string text, 
							   uint16_t	scrollspeed_ms /* When the scrollspeed is 0, then there is no scrolling */)
{
	if (text.size() == 0)
		throw fullcircle::RenderException("No text to render was given");
	
	int text_screen_width = _width;
		
	if (scrollspeed_ms > 0)
	{
		Frame::Ptr item = searchCharacter('M'); // this is the widest character
		if (item->width() == 0 && item->height() == 0)
			throw fullcircle::RenderException("We have big problems!");
		text_screen_width = text.size() * (item->width() + 1) /*we need some space after the last character*/ + x_offset;
	}
	
	Frame::Ptr screen(new Frame(text_screen_width, _height));
	screen->fill_whole(COLOR_TRANSPARENT);	
	std::cout << "We want to print: " << text << std::endl;
	
	int xcount = 0;
	for (uint16_t i=0; i < text.size(); i++) {
		Frame::Ptr item = searchCharacter(text[i]);
		if (item == 0)
		{
			throw fullcircle::RenderException("Could not find all character representation");
		}
		screen->set_pixel(x_offset + xcount, y_offset, item);
		xcount += item->width();
	}
	if (scrollspeed_ms == 0)
	{   // add the simple text (when there is no scrolling)
		sequence->add_frame(screen);
	}
	else
	{
		uint32_t timePerFrame = (sequence->fps() * scrollspeed_ms / 1000);
		Frame::Ptr actual_screen_part(new Frame(_width, _height));
		for (uint16_t i=0; i < text_screen_width - _width; i++) {
			actual_screen_part->set_pixel_window(i, 0, screen);
			Frame::Ptr copy(new Frame(actual_screen_part->width(), actual_screen_part->height()));
			copy->set_pixel(0,0,actual_screen_part); // make a deep copy for the sequence
			for (uint32_t time=0; time < timePerFrame; time++) {
				sequence->add_frame(copy);
			}
		}
		sequence->trim_end();
	}
}

void FontRenderer::scroll_text(Sequence::Ptr sequence, uint16_t x_offset, uint16_t y_offset, std::string text, 
							   uint16_t	scrollspeed_inms, RGB_t textColor)
{
	uint32_t startFrame=sequence->size();
	scroll_text(sequence, x_offset, y_offset, text, scrollspeed_inms);
	uint32_t length=sequence->size();
	for (; startFrame < length; startFrame++) {
		sequence->get_frame(startFrame)->swap_color(COLOR_SET, textColor);
	}	
}

Frame::Ptr FontRenderer::searchCharacter(char c)
{
	fullcircle::SpriteIO::Ptr sprite_io(new fullcircle::SpriteIO());
	QString resource;
	switch (c) {
		case ' ':
			resource.sprintf(":/sprites/fonts/font1_%dx%d_SPACE.png", _font_width, _font_height);
			break;
		default: // all normal characters
			resource.sprintf(":/sprites/fonts/font1_%dx%d_%c.png", _font_width, _font_height, c);
			break;
	}
//	printf("Ressource for %c is %s\n", c, qPrintable( resource ));
	return sprite_io->load(std::string(resource.toAscii()));
}
