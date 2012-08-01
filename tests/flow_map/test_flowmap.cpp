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

#define BOOST_TEST_MODULE store_test
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include <testconfig.h>
#include <boost/filesystem.hpp>
#include <libfullcircle/frame.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/sequence.pb.h>
#include <libfullcircle/flowmap.hpp>

fullcircle::RGB_t white;
fullcircle::RGB_t black;
fullcircle::RGB_t red;

void init_color() {
  white.red = white.green = white.blue = 255;
  black.red = black.green = black.blue = 0;
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

BOOST_AUTO_TEST_CASE ( check_flowmap_creating ) {
	init_color();
	
	fullcircle::RGB_t color1;
	color1.red = 128;
	color1.green = 255;
	color1.blue = 160;
	
	fullcircle::Frame::Ptr hills(new fullcircle::Frame(3,3));
	hills->fill_whole(color1);	
    std::cout << "Test hill:" << std::endl;
	hills->dump_frame(std::cout);	
	fullcircle::FlowMap::Ptr fm(new fullcircle::FlowMap());
	fm->init(hills);
	
	BOOST_CHECK_EQUAL (3, hills->width()  );
	BOOST_CHECK_EQUAL (3, hills->height() );
	
	// reuse the variable hill to specify the start points.
	hills->fill_whole(black);
	hills->set_pixel(1, 1, 0, 180, 0);
	std::cout << "start color:" << std::endl;
	hills->dump_frame(std::cout);
	fm->start_points(hills);
}