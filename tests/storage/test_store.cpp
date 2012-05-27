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
#include <libfullcircle/sequence.pb.h>
#include <boost/filesystem.hpp>

namespace bfs=boost::filesystem;

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
  std::cout << "### Testing a single frame." << std::endl;
  fullcircle::RGB_t white;
  white.red = white.green = white.blue = 255;

  fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
  f1->set_pixel(0, 0, 255, 255, 255);
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

  std::cout << "### Testing equality operators" << std::endl; 
  fullcircle::Frame::Ptr f2(new fullcircle::Frame(4,4));
  for (uint8_t x=0; x < f2->x_dim(); ++x) {
    for (uint8_t y=0; y < f2->y_dim(); ++y) {
      f2->set_pixel(x, y, white);
    }
  }
  f2->dump_frame(std::cout);
  BOOST_REQUIRE(*f2 == *f2);
  BOOST_REQUIRE(*f1 != *f2);

  std::cout << "### Testing copy constructor" << std::endl; 
  fullcircle::Frame::Ptr f3(new fullcircle::Frame(*f2));
  f3->dump_frame(std::cout);
  BOOST_REQUIRE(*f3 == *f2);
  BOOST_REQUIRE(*f1 != *f3);

}

BOOST_AUTO_TEST_CASE ( check_sequence ) {
  fullcircle::RGB_t white;
  white.red = white.green = white.blue = 255;
  fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));
  fullcircle::Frame::Ptr f0(new fullcircle::Frame(4,4));
  f0->set_pixel(0, 0, white);
  fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
  f1->set_pixel(1, 0, white);
  fullcircle::Frame::Ptr f2(new fullcircle::Frame(4,4));
  f2->set_pixel(2, 0, white);
  fullcircle::Frame::Ptr f3(new fullcircle::Frame(4,4));
  f3->set_pixel(3, 0, white);
  
  seq->add_frame(f0);
  seq->add_frame(f1);
  seq->add_frame(f2);
  seq->add_frame(f3);
  std::cout << "Printing some test frames" << std::endl;
  seq->dump(std::cout);

  BOOST_REQUIRE(seq->size() == 4);

  fullcircle::Frame::Ptr damaged_frame(new fullcircle::Frame(3,4));
  try {
    seq->add_frame(damaged_frame);
    BOOST_FAIL( "Was able to add a damaged frame." );
  } catch (fullcircle::GenericException const& ex) {
    std::cout << "Caught expected exception: " << ex.what() << std::endl;
  }
  BOOST_REQUIRE(seq->size() == 4);

  std::cout << "Testing our sequence iterator" << std::endl;

  fullcircle::Sequence::const_iterator it;
  uint8_t framecount=0;
  for (it = seq->begin(); it != seq->end(); ++it) {
    (*it)->dump_frame(std::cout);
    framecount++;
  }
  BOOST_REQUIRE(seq->size() == framecount);

}


BOOST_AUTO_TEST_CASE ( check_sequence_storage ) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  bfs::path binfile(TEST_SEQUENCE_FILE);
  
  // create a test sequence
  fullcircle::RGB_t white;
  white.red = white.green = white.blue = 255;
  fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));
  fullcircle::Frame::Ptr f0(new fullcircle::Frame(4,4));
  f0->set_pixel(0, 0, white);
  fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
  f1->set_pixel(1, 0, white);
  fullcircle::Frame::Ptr f2(new fullcircle::Frame(4,4));
  f2->set_pixel(2, 0, white);
  fullcircle::Frame::Ptr f3(new fullcircle::Frame(4,4));
  f3->set_pixel(3, 0, white);
  seq->add_frame(f0);
  seq->add_frame(f1);
  seq->add_frame(f2);
  seq->add_frame(f3);

  std::cout << "Saving sequence to file " << binfile << std::endl;
  std::fstream output(binfile.c_str(), 
      std::ios::out | std::ios::trunc | std::ios::binary);
  seq->save(output, "Testcase: check_sequence_storage", "current");
  output.close();

  std::cout << "Loading sequence from file " << binfile << std::endl;
  std::fstream input(binfile.c_str(), 
      std::ios::in | std::ios::binary);
  try {
    fullcircle::Sequence::Ptr loaded_seq(new fullcircle::Sequence(input));
    std::cout << "Loaded sequence:" << std::endl;
    loaded_seq->dump(std::cout);
    BOOST_REQUIRE(*seq == *loaded_seq);
  } catch (fullcircle::GenericException& ex) {
    std::cout << "Caught exception: " << ex.what() << std::endl;
    BOOST_FAIL( "Wasn't able to load sequence." );
  }
  input.close();



  google::protobuf::ShutdownProtobufLibrary();
}
//BOOST_AUTO_TEST_SUITE_END()
