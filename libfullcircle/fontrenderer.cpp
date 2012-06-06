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
};

void FontRenderer::write_text(Sequence::Ptr sequence, uint16_t x, uint16_t y, std::string text)
{
	scroll_text(sequence, x, y, text, 0);
}

void FontRenderer::write_text(Sequence::Ptr sequence, uint16_t x, uint16_t y, std::string text, RGB_t textColor)
{
	uint32_t startFrame=sequence->size();
	write_text(sequence, x, y, text);
	uint32_t length=sequence->size();
	for (; startFrame < length; startFrame++) {
		sequence->get_frame(startFrame)->swap_color(COLOR_SET, textColor);
	}
}

void FontRenderer::scroll_text(Sequence::Ptr sequence, uint16_t x, uint16_t y, std::string text, 
							   uint16_t	scrollspeed_ms)
{
	if (text.size() == 0)
		throw fullcircle::RenderException("No text to render was given");
	
	int text_screen_width = _width;
		
	if (scrollspeed_ms > 0)
	{
		// take the width of the first item (the same for all, because we have monospace)
		Frame::Ptr item = searchCharacter(text[0]);
		if (item->width() == 0 && item->height() == 0)
			throw fullcircle::RenderException("We have big problems!");
		text_screen_width = text.size() * item->width() + _width /*we need a white page for last page*/;
	}
	
	Frame::Ptr screen(new Frame(text_screen_width, _height));
	screen->fill_whole(COLOR_TRANSPARENT);	
	std::cout << "We want to print: " << text << std::endl;
	
	for (uint16_t i=0; i < text.size(); i++) {
		Frame::Ptr item = searchCharacter(text[i]);
		if (item == 0)
		{
			throw fullcircle::RenderException("Could not find all character representation");
		}
		screen->set_pixel(i*item->width(),0, item);
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
			actual_screen_part->set_pixel_window(i,0,screen);
			Frame::Ptr copy(new Frame(actual_screen_part->width(), actual_screen_part->height()));
			copy->set_pixel(0,0,actual_screen_part); // make a deep copy for the sequence
			for (uint32_t time=0; time < timePerFrame; time++) {
				sequence->add_frame(copy);
			}
			std::cout << "position is " << (i + 1)  << " is : " << std::endl;
			actual_screen_part->dump_frame(std::cout);
		}
	}
}

void FontRenderer::scroll_text(Sequence::Ptr sequence, uint16_t x, uint16_t y, std::string text, 
							   uint16_t	scrollspeed_inms, RGB_t textColor)
{
	uint32_t startFrame=sequence->size();
	scroll_text(sequence, x, y, text, scrollspeed_inms);
	uint32_t length=sequence->size();
	for (; startFrame < length; startFrame++) {
		sequence->get_frame(startFrame)->swap_color(COLOR_SET, textColor);
	}	
}

Frame::Ptr FontRenderer::searchCharacter(char c)
{
	fullcircle::SpriteIO::Ptr sprite_io(new fullcircle::SpriteIO());
	QString resource;
	resource.sprintf(":/sprites/fonts/font1_5x5_%c.png", c);
	printf("Ressource for %c is %s\n", c, qPrintable( resource ));
	return sprite_io->load(std::string(resource.toAscii()));
}
