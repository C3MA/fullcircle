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
#include <libfullcircle/frame.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/sprite_io.hpp>
#include <boost/filesystem.hpp>
#include <QApplication>

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

BOOST_AUTO_TEST_CASE ( check_test_sprite ) {
  init_color();
  Q_INIT_RESOURCE(sprites);
  std::cout << "### Testing sprites." << std::endl;
 
  fullcircle::SpriteIO::Ptr sprite_io(new fullcircle::SpriteIO());
  fullcircle::Frame::Ptr f1(
      sprite_io->load(std::string(":/sprites/test_sprite_2x2.png")));
  f1->dump_frame(std::cout);

  fullcircle::RGB_t setcolor0=f1->get_pixel(1,0);
  BOOST_REQUIRE(setcolor0.red == 252);
  BOOST_REQUIRE(setcolor0.green == 255);
  BOOST_REQUIRE(setcolor0.blue == 255);

  fullcircle::RGB_t setcolor1=f1->get_pixel(0,1);
  BOOST_REQUIRE(setcolor1.red == 252);
  BOOST_REQUIRE(setcolor1.green == 255);
  BOOST_REQUIRE(setcolor1.blue == 255);

  fullcircle::RGB_t setcolor2=f1->get_pixel(1,1);
  BOOST_REQUIRE(setcolor2.red == 0);
  BOOST_REQUIRE(setcolor2.green == 0);
  BOOST_REQUIRE(setcolor2.blue == 0);

  fullcircle::RGB_t setcolor3=f1->get_pixel(0,0);
  BOOST_REQUIRE(setcolor3.red == 0);
  BOOST_REQUIRE(setcolor3.green == 0);
  BOOST_REQUIRE(setcolor3.blue == 0);

}


//BOOST_AUTO_TEST_SUITE_END()
