#include "fontrenderer.hpp"
#include <iostream>
#include <boost/spirit.hpp> 
#include <boost/assign/std/vector.hpp>
#include <string>
#include <set>
#include <sstream>
#include <exception>
#include <fstream>
#include <boost/format.hpp>

using namespace boost::assign; // bring 'operator+()' into scope [needed for vector]
using namespace fullcircle;

typedef struct {
    uint8_t ascii;
	std::vector<uint16_t> map;
} AsciiMapping_t;

/*** This is not the best postion for this code, becase this makes it not threadsafe ***/
std::vector<uint16_t> fullcircle::gVars;
int fullcircle::gAsciiChar = 0;
std::vector<AsciiMapping_t> fullcircle::gCompleteMap;

struct xbmtext_grammar 
: public boost::spirit::grammar<xbmtext_grammar> 
{ 
	/******* these functions are neeeded to store the extracted data **********/
	struct set_asciiChar 
	{ 		
		void operator()(const int d) const 
		{ 
			gAsciiChar = d;			
		} 
	};
	
	struct number_add {
		void operator()(const int d) const
		{
			gVars += d;
		}
	};
	
	struct map_add {
		void operator()(const char *begin, const char *end) const 
		{			
			AsciiMapping_t map;
			map.ascii = gAsciiChar;
			map.map = gVars;
			gCompleteMap += map;
			gAsciiChar = 0;
			gVars.clear();
		}
	};
	
	
	/******* The real scanning algorithm ************/
	template <typename Scanner> 
	struct definition 
	{ 
		boost::spirit::rule<Scanner> object, value, descval, desckey, description, asciinum, number, image, mapping, string, comment;
		
		definition(const xbmtext_grammar &self) 
		{ 	
			using namespace boost::spirit; 
			// basic datatypes:
			string = "\"" >> *~ch_p("\"") >> "\"";
			
			mapping = (asciinum >> ":" >> image)[map_add()];
			image = "[" >> number >> *("," >> number) >> "]"; 
			number = int_p[number_add()];
			asciinum = "\"" >> int_p[set_asciiChar()] >> "\"";
			desckey = string;
			descval = string;
			description = desckey >> ":" >> descval;
			value = mapping | description;
			comment = "#" >> *~ch_p("{");
			object = *(comment) >> "{" >> value >> *("," >> value) >> "}";			
		} 
		
		const boost::spirit::rule<Scanner> &start() 
		{ 
			return object; 
		}
		
	};
};

FontRenderer::FontRenderer ( uint16_t width, uint16_t height)
  : _width(width)
  , _height(height)
{

};

void FontRenderer::load_font(std::string font_file) {	
	std::ifstream t(font_file.c_str());
	if(!t)
    {
        std::cerr<<"error"<<std::endl;
        return;
    }
	
	std::stringstream buffer;
	buffer << t.rdbuf();
	// Now the whole file is in a buffer :-)
	
	xbmtext_grammar g; 
	boost::spirit::parse_info<> pi = boost::spirit::parse(buffer.str().c_str(), g, boost::spirit::space_p); 
	if (pi.hit) 
	{ 
		if (!pi.full)
			throw fullcircle::DataFormatException("parsing data partially");
		
		/******* The data was extacted, so lets convert it ********/
		
		/*** Search for the width of one character (we have to search the widest) ****/
		uint16_t item, count, maxWidth = 0, height, maxHeight = 0;
		for (unsigned int i=0; i < gCompleteMap.size(); i++) {
			height=0;
			for (unsigned int j=0; j < gCompleteMap[i].map.size(); j++) {
				item = gCompleteMap[i].map[j];
				count=0;
				if (item > 0)
					height++;
				while (item > 0) {
					item = item >> 1;
					count++;
				}
				if (count > maxWidth)
				{
					maxWidth = count;
//					printf("maximum width is %2d found at %c\n",maxWidth, completeMap[i].ascii);
					if (maxWidth > _width)
					{
						std::string msg = str( boost::format("maximum height overflow of %1 found at %2\n")
											% maxWidth % gCompleteMap[i].ascii);
						throw fullcircle::DataFormatException(msg);
					}
				}
			}
			if (height > maxHeight)
			{
				maxHeight = height;
//				printf("maximum height is %2d found at %c\n",maxHeight, completeMap[i].ascii);
				if (maxHeight > _height)
				{
					std::string msg = str (boost::format("maximum height overflow of %1 found at %2\n")
									% maxHeight % gCompleteMap[i].ascii);
					throw fullcircle::DataFormatException(msg);
				}
			}
		}
		
		/*********** Now we can create the frames with the binary representation of the characters *********/
		for (unsigned int i=0; i < gCompleteMap.size(); i++) {
			Frame::Ptr repesentation(new Frame(maxWidth, maxHeight));
			for (unsigned int y=0; y < maxHeight; y++) {
				item = gCompleteMap[i].map[y];
				count=0;
				while (item > 0) {
					if (item & 1 == 1) {
						repesentation->set_pixel(count, y, COLOR_TRANSPARENT);
					} else {
						repesentation->set_pixel(count, y, COLOR_SET);
					}
					item = item >> 1;
					count++;
				}
			}
			_asciiMapping[gCompleteMap[i].ascii] = repesentation;
		}
	}
	else
	{
		std::string msg = ((std::string) "parsing failed; stopped at '") + pi.stop + "'";
		throw fullcircle::DataFormatException(msg);
	}
}

void FontRenderer::write_text(Sequence::Ptr sequence, uint16_t x, uint16_t y, std::string text) {
	
	Frame::Ptr screen(new Frame(_width, _height));
	screen->fill_whole(COLOR_TRANSPARENT);	
	std::cout << "We want to print: " << text << std::endl;
	
	for (uint16_t i=0; i < text.size(); i++) {
		Frame::Ptr item = _asciiMapping[text[i]];
		if (item == 0)
		{
			throw fullcircle::RenderException("Could not find all character representation");
		}
		std::cout << text[i] << " is : " << std::endl;
		item->dump_frame(std::cout);
		screen->set_pixel(i*item->width(),0, item);
		sequence->add_frame(screen);
	}
}

