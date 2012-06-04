#include "fontrenderer.hpp"
#include <iostream>
#include <boost/spirit.hpp> 
#include <boost/assign/std/vector.hpp>
#include <string>
#include <set>
#include <sstream>
#include <exception>
#include <fstream>


using namespace boost::assign; // bring 'operator+()' into scope [needed for vector]
using namespace fullcircle;

/*** This is not the best postion for this code, becase this makes it not threadsafe ***/
std::vector<int> fullcircle::vars;
int fullcircle::asciiChar = 0;

struct xbmtext_grammar 
: public boost::spirit::grammar<xbmtext_grammar> 
{ 
	/******* these functions are neeeded to store the extracted data **********/
	struct set_asciiChar 
	{ 		
		void operator()(const int d) const 
		{ 
			asciiChar = d;			
		} 
	};
	
	struct number_add {
		void operator()(const int d) const
		{
			vars += d;
		}
	};
	
	struct map_add {
		void operator()(const char *begin, const char *end) const 
		{
			std::cout << asciiChar << " = ";
			for (unsigned int i=0; i < vars.size(); i++) {
				std::cout << vars[i] << " ";
			}
			std::cout << std::endl;
			vars.clear();
		}
	};
	
	
	/******* The real scanning algorithm ************/
	template <typename Scanner> 
	struct definition 
	{ 
		boost::spirit::rule<Scanner> object, value, descval, desckey, description, asciinum, number, image, mapping, string;
		
		definition(const xbmtext_grammar &self) 
		{ 
			
			using namespace boost::spirit; 
			// basic datatypes:
			string = "\"" >> *~ch_p("\"") >> "\"";
			
			mapping = (asciinum >> ":" >> image)[map_add()];
			image = "[" >> number >> *("," >> number) >> "]"; 
			number = int_p[number_add()];
			asciinum = 
			
			(	"\"" >> int_p[set_asciiChar()] >> "\"" )
			;
			desckey = string;
			descval = string;
			description = desckey >> ":" >> descval;
			value = mapping | description;
			object = "{" >> value >> *("," >> value) >> "}"; 
			
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
		if (pi.full) 
			std::cout << "parsing all data successfully" << std::endl; 
		else 
			std::cout << "parsing data partially" << std::endl; 
		std::cout << pi.length << " characters parsed" << std::endl;
		//FIXME here is something to do !
	} 
	else 
		std::cout << "parsing failed; stopped at '" << pi.stop << "'" << std::endl; 
}

void FontRenderer::write_text(Sequence::Ptr sequence, uint16_t x, uint16_t y, std::string text) {
	//FIXME here is something to do !
}

