/**
 * This file is part of libfullcircle.
 *
 * (c) Mathias Dalheimer <md@gonium.net>, 2012
 *
 * libfullcircle is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libfullcircle is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libfullcircle. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define BOOST_TEST_MODULE fillframe_test
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include <testconfig.h>
#include <libfullcircle/frame.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/sequence.pb.h>
#include <boost/filesystem.hpp>

namespace bfs=boost::filesystem;
fullcircle::RGB_t white;
fullcircle::RGB_t red;

void init_color() {
  white.red = white.green = white.blue = 255;
  red.red = 255; red.green = red.blue = 0;
}

/**
 * see http://www.boost.org/doc/libs/1_43_0/libs/test/doc/html/tutorials/hello-the-testing-world.html
 */

BOOST_AUTO_TEST_CASE ( check_sanity ) {
  try {
    std::cout << "Demo test case: Checking world sanity." << std::endl;
    BOOST_CHECK_EQUAL (42, 42);
    BOOST_CHECK( 23 != 42 );        // #1 continues on error
    BOOST_REQUIRE( 23 != 42 );      // #2 throws on error

  } catch (std::exception const & ex) {
    BOOST_ERROR ( ex.what() );
  }
  if( 23 == 42 ) {
    BOOST_FAIL( "23 == 42, oh noes");             // #4 throws on error
  }
}


BOOST_AUTO_TEST_CASE ( check_gradient ) {
	fullcircle::RGB_t from, to;
	from.blue = 255; from.red = from.green = 0;
	to.blue = 1; to.red = to.green = 0;
	fullcircle::Frame::Ptr frame(new fullcircle::Frame(10,5));
	frame->fill_gradient_horizontal(from, to);		
	frame->dump_frame(std::cout);
	fullcircle::RGB_t pixel = frame->get_pixel(0, 0);
	BOOST_CHECK_EQUAL (pixel.blue, from.blue);
	BOOST_CHECK_EQUAL (pixel.green, from.green);
	BOOST_CHECK_EQUAL (pixel.red, from.red);
	// Check last pixel
	pixel = frame->get_pixel(frame->width()-1, 0);
	BOOST_CHECK_EQUAL (pixel.blue, to.blue);
	BOOST_CHECK_EQUAL (pixel.green, to.green);
	BOOST_CHECK_EQUAL (pixel.red, to.red);
}



BOOST_AUTO_TEST_CASE ( check_frame_add ) {
  std::cout << "### testing PLUS operator for frames" << std::endl;
	fullcircle::RGB_t color1, color2, pixel;
	color1.blue = 255; color1.red = color1.green = 0;
	fullcircle::Frame::Ptr frame(new fullcircle::Frame(10,5));
	frame->fill_whole(color1);		
  std::cout << "First frame:" << std::endl;
  frame->dump_frame(std::cout);
	
	color2.blue = color2.red = 0; color2.green = 255;
	fullcircle::Frame::Ptr frame2(new fullcircle::Frame(10,5));
	frame->fill_whole(color2);		
  std::cout << "Second frame:" << std::endl;
  frame2->dump_frame(std::cout);
	
	fullcircle::Frame::Ptr sum = (*frame) + frame2;
  std::cout << "Sum of two frames:" << std::endl;
  sum->dump_frame(std::cout);
	
	// Check last pixel
	pixel = sum->get_pixel(0, 0);
	BOOST_CHECK_EQUAL (pixel.blue, 255);
	BOOST_CHECK_EQUAL (pixel.green, 255);
	BOOST_CHECK_EQUAL (pixel.red, 0);
}


//BOOST_AUTO_TEST_SUITE_END()
