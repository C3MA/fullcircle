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
#include <testconfig.h>
#include <libfullcircle/frame.hpp>


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

BOOST_AUTO_TEST_CASE ( check_frame ) {
  std::cout << "Testing a single frame." << std::endl;
  fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));

  f1->set_pixel(0, 0, 255, 255, 255);
  fullcircle::RGB_t white;
  white.red = white.green = white.blue = 255;
  f1->set_pixel(1, 0, white);

  f1->dump_frame(std::cout);

  fullcircle::RGB_t setcolor0=f1->get_pixel(0,0);
  BOOST_REQUIRE(setcolor0.red == 255);
  BOOST_REQUIRE(setcolor0.green == 255);
  BOOST_REQUIRE(setcolor0.blue == 255);

  fullcircle::RGB_t setcolor1=f1->get_pixel(1,0);
  BOOST_REQUIRE(setcolor1.red == 255);
  BOOST_REQUIRE(setcolor1.green == 255);
  BOOST_REQUIRE(setcolor1.blue == 255);

  fullcircle::RGB_t setcolor2=f1->get_pixel(0,1);
  BOOST_REQUIRE(setcolor2.red == 0);
  BOOST_REQUIRE(setcolor2.green == 0);
  BOOST_REQUIRE(setcolor2.blue == 0);

  try {
    f1->set_pixel(10,10,white);
    BOOST_FAIL( "Was able to set pixel outside of the frame." );
  } catch (fullcircle::GenericException const& ex) {
    std::cout << "Caught expected exception: " << ex.what() << std::endl;
  }

  try {
    f1->set_pixel(-1,-1,white);
    BOOST_FAIL( "Was able to set pixel outside of the frame." );
  } catch (fullcircle::GenericException const& ex) {
    std::cout << "Caught expected exception: " << ex.what() << std::endl;
  }
}
//  bfs::path db(TEST_DB_FILE);

//BOOST_AUTO_TEST_SUITE_END()
