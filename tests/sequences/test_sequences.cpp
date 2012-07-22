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

BOOST_AUTO_TEST_CASE ( check_frame ) {
  init_color();
  std::cout << "### Testing a single frame." << std::endl;
  fullcircle::RGB_t white;
  white.red = white.green = white.blue = 255;

  fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
  f1->set_pixel(0, 0, 255, 255, 255);
  f1->set_pixel(1, 0, white);
  f1->set_pixel(2, 0, red);

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

  fullcircle::RGB_t setcolor3=f1->get_pixel(2,0);
  BOOST_REQUIRE(setcolor3.red == 255);
  BOOST_REQUIRE(setcolor3.green == 0);
  BOOST_REQUIRE(setcolor3.blue == 0);


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
  for (uint8_t x=0; x < f2->width(); ++x) {
    for (uint8_t y=0; y < f2->height(); ++y) {
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
  init_color();
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
  f3->set_pixel(3, 0, red);

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

  std::cout << "Testing sequence concatenation with the APPEND operator" << std::endl;
  fullcircle::Sequence::Ptr big_seq;
  big_seq = (*seq) << seq;
  BOOST_REQUIRE(big_seq->size() == 2*seq->size());

  fullcircle::Sequence::Ptr damaged_seq(new fullcircle::Sequence(2,4,4));
  fullcircle::Frame::Ptr f5(new fullcircle::Frame(4,4));
  damaged_seq->add_frame(f5);
   try {
    big_seq = (*big_seq) << damaged_seq;
    BOOST_FAIL( "Was able to add a damaged sequence." );
  } catch (fullcircle::GenericException const& ex) {
    std::cout << "Caught expected exception: " << ex.what() << std::endl;
  }
  
}


BOOST_AUTO_TEST_CASE ( check_sequence_storage ) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  bfs::path binfile(TEST_SEQUENCE_FILE);
  init_color();
  // create a test sequence
  fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,5,4));
  fullcircle::Frame::Ptr f0(new fullcircle::Frame(5,4));
  f0->set_pixel(0, 0, white);
  fullcircle::Frame::Ptr f1(new fullcircle::Frame(5,4));
  f1->set_pixel(1, 0, red);
  fullcircle::Frame::Ptr f2(new fullcircle::Frame(5,4));
  f2->set_pixel(2, 0, white);
  fullcircle::Frame::Ptr f3(new fullcircle::Frame(5,4));
  f3->set_pixel(3, 0, red);
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

BOOST_AUTO_TEST_CASE ( check_sequence_trim ) {
	init_color();
	std::cout << "### Testing trim the end of a sequence." << std::endl;
	fullcircle::RGB_t white;
	white.red = white.green = white.blue = 255;
	
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));
	fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty2(new fullcircle::Frame(4,4));
	f1->set_pixel(0, 0, 255, 255, 255);
	f1->set_pixel(1, 0, white);
	f1->set_pixel(2, 0, red);
	seq->add_frame(f1);
	seq->add_frame(f_empty);
	seq->add_frame(f_empty);
	seq->add_frame(f_empty2);
	seq->add_frame(f_empty2);
	seq->dump(std::cout);
	seq->trim_end();
    BOOST_CHECK_EQUAL (1, seq->size());
	seq->dump(std::cout);	
}

BOOST_AUTO_TEST_CASE ( check_sequence_add ) {
	init_color();
	std::cout << "################## Testing the addition of two sequences." << std::endl;
	fullcircle::RGB_t color1, pixel;
	color1.red = color1.green = color1.blue = 255;
	
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));
	fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty(new fullcircle::Frame(4,4));
	f1->fill_whole(color1);
	seq->add_frame(f1);
	seq->add_frame(f_empty);
	seq->add_frame(f_empty);
	//seq->dump(std::cout);
	
	fullcircle::Sequence::Ptr seq2(new fullcircle::Sequence(10,4,4));
    fullcircle::Frame::Ptr f2(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty2(new fullcircle::Frame(4,4));
	f2->fill_whole(color1);
	seq2->add_frame(f_empty2);
	seq2->add_frame(f2);
	seq2->add_frame(f_empty2);
	//seq2->dump(std::cout);
	
	fullcircle::Sequence::Ptr sum = (*seq) + seq2;
	
    BOOST_CHECK_EQUAL (3, sum->size());
	pixel = sum->get_frame(0)->get_pixel(0, 0); // fetch the first pixel from the first frame
	BOOST_CHECK_EQUAL (pixel.red, 255);
	BOOST_CHECK_EQUAL (pixel.green, 255);
	BOOST_CHECK_EQUAL (pixel.blue, 255);
	pixel = sum->get_frame(1)->get_pixel(0, 0); // fetch the first pixel from the second frame
	BOOST_CHECK_EQUAL (pixel.red, 255);
	BOOST_CHECK_EQUAL (pixel.green, 255);
	BOOST_CHECK_EQUAL (pixel.blue, 255);
	pixel = sum->get_frame(2)->get_pixel(0, 0); // fetch the first pixel from the third frame
	BOOST_CHECK_EQUAL (pixel.red, 0);
	BOOST_CHECK_EQUAL (pixel.green, 0);
	BOOST_CHECK_EQUAL (pixel.blue, 0);	
	//std::cout << "####### The sum is the following:" << std::endl;
	//sum->dump(std::cout);	
}

BOOST_AUTO_TEST_CASE ( check_sequence_add_overflow ) {
	init_color();
	std::cout << "################## Testing the addition of two sequences (with overflow in the first frame)." << std::endl;
	fullcircle::RGB_t color1, pixel;
	color1.red = color1.green = color1.blue = 160;
	
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));
	fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty(new fullcircle::Frame(4,4));
	f1->fill_whole(color1);
	seq->add_frame(f1);
	seq->add_frame(f_empty);
	seq->add_frame(f1);
	//seq->dump(std::cout);
	
	fullcircle::Sequence::Ptr seq2(new fullcircle::Sequence(10,4,4));
    fullcircle::Frame::Ptr f2(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty2(new fullcircle::Frame(4,4));
	f2->fill_whole(color1);
	seq2->add_frame(f2);
	seq2->add_frame(f2);
	seq2->add_frame(f_empty2);
	//seq2->dump(std::cout);
	
	fullcircle::Sequence::Ptr sum = (*seq) + seq2;
	
    BOOST_CHECK_EQUAL (3, sum->size());
	pixel = sum->get_frame(0)->get_pixel(0, 0); // fetch the first pixel from the first frame
	BOOST_CHECK_EQUAL (pixel.red, 255);
	BOOST_CHECK_EQUAL (pixel.green, 255);
	BOOST_CHECK_EQUAL (pixel.blue, 255);
	pixel = sum->get_frame(1)->get_pixel(0, 0); // fetch the first pixel from the second frame
	BOOST_CHECK_EQUAL (pixel.red, 160);
	BOOST_CHECK_EQUAL (pixel.green, 160);
	BOOST_CHECK_EQUAL (pixel.blue, 160);
	pixel = sum->get_frame(2)->get_pixel(0, 0); // fetch the first pixel from the third frame
	BOOST_CHECK_EQUAL (pixel.red, 160);
	BOOST_CHECK_EQUAL (pixel.green, 160);
	BOOST_CHECK_EQUAL (pixel.blue, 160);	
	//std::cout << "####### The sum is the following:" << std::endl;
	//sum->dump(std::cout);	
}

BOOST_AUTO_TEST_CASE ( check_sequence_add_different_length ) {
	init_color();
	std::cout << "################## Testing the addition of two sequences (with different lengths)." << std::endl;
	fullcircle::RGB_t color1, pixel;
	color1.red = color1.green = color1.blue = 160;
	
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));
	fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty(new fullcircle::Frame(4,4));
	f1->fill_whole(color1);
	seq->add_frame(f1);
	seq->add_frame(f_empty);
	seq->add_frame(f1);
	//seq->dump(std::cout);
	
	fullcircle::Sequence::Ptr seq2(new fullcircle::Sequence(10,4,4));
    fullcircle::Frame::Ptr f2(new fullcircle::Frame(4,4));
	f2->fill_whole(color1);
	seq2->add_frame(f2);
	seq2->add_frame(f2);
	//seq2->dump(std::cout);
	
	fullcircle::Sequence::Ptr sum = (*seq) + seq2;
	
    BOOST_CHECK_EQUAL (3, sum->size());
	pixel = sum->get_frame(0)->get_pixel(0, 0); // fetch the first pixel from the first frame
	BOOST_CHECK_EQUAL (pixel.red, 255);
	BOOST_CHECK_EQUAL (pixel.green, 255);
	BOOST_CHECK_EQUAL (pixel.blue, 255);
	pixel = sum->get_frame(1)->get_pixel(0, 0); // fetch the first pixel from the second frame
	BOOST_CHECK_EQUAL (pixel.red, 160);
	BOOST_CHECK_EQUAL (pixel.green, 160);
	BOOST_CHECK_EQUAL (pixel.blue, 160);
	pixel = sum->get_frame(2)->get_pixel(0, 0); // fetch the first pixel from the third frame
	BOOST_CHECK_EQUAL (pixel.red, 160);
	BOOST_CHECK_EQUAL (pixel.green, 160);
	BOOST_CHECK_EQUAL (pixel.blue, 160);	
	//std::cout << "####### The sum is the following:" << std::endl;
	//sum->dump(std::cout);	
}

BOOST_AUTO_TEST_CASE ( check_sequence_add_different_length2 ) {
	init_color();
	std::cout << "################## Testing the addition of two sequences (with different length of frames, second variation)." << std::endl;
	fullcircle::RGB_t color1, pixel;
	color1.red = color1.green = color1.blue = 160;
	
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));
	fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty(new fullcircle::Frame(4,4));
	f1->fill_whole(color1);
	seq->add_frame(f1);
	seq->add_frame(f_empty);
	//seq->dump(std::cout);
	
	fullcircle::Sequence::Ptr seq2(new fullcircle::Sequence(10,4,4));
    fullcircle::Frame::Ptr f2(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty2(new fullcircle::Frame(4,4));
	f2->fill_whole(color1);
	seq2->add_frame(f2);
	seq2->add_frame(f2);
	seq2->add_frame(f_empty2);
	//seq2->dump(std::cout);
	
	fullcircle::Sequence::Ptr sum = (*seq) + seq2;
	
    BOOST_CHECK_EQUAL (3, sum->size());
	pixel = sum->get_frame(0)->get_pixel(0, 0); // fetch the first pixel from the first frame
	BOOST_CHECK_EQUAL (pixel.red, 255);
	BOOST_CHECK_EQUAL (pixel.green, 255);
	BOOST_CHECK_EQUAL (pixel.blue, 255);
	pixel = sum->get_frame(1)->get_pixel(0, 0); // fetch the first pixel from the second frame
	BOOST_CHECK_EQUAL (pixel.red, 160);
	BOOST_CHECK_EQUAL (pixel.green, 160);
	BOOST_CHECK_EQUAL (pixel.blue, 160);
	pixel = sum->get_frame(2)->get_pixel(0, 0); // fetch the first pixel from the third frame
	BOOST_CHECK_EQUAL (pixel.red, 0);
	BOOST_CHECK_EQUAL (pixel.green, 0);
	BOOST_CHECK_EQUAL (pixel.blue, 0);	
	//std::cout << "####### The sum is the following:" << std::endl;
	//sum->dump(std::cout);	
}

BOOST_AUTO_TEST_CASE ( check_sequence_add_offset ) {
	init_color();
	std::cout << "################## Testing the addition of two sequences (with an offset)." << std::endl;
	fullcircle::RGB_t color1, pixel;
	color1.red = color1.green = color1.blue = 160;
	
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));
	fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty(new fullcircle::Frame(4,4));
	f1->fill_whole(color1);
	seq->add_frame(f1);
	seq->add_frame(f1);
//	seq->dump(std::cout);
	
	fullcircle::Sequence::Ptr seq2(new fullcircle::Sequence(10,4,4));
    fullcircle::Frame::Ptr f2(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty2(new fullcircle::Frame(4,4));
	f2->fill_whole(color1);
	seq2->add_frame(f2);
	seq2->add_frame(f2);
	seq2->add_frame(f_empty2);
//	seq2->dump(std::cout);
	
	fullcircle::Sequence::Ptr sum = seq->add(1, seq2);
	
    BOOST_CHECK_EQUAL (4, sum->size());
	pixel = sum->get_frame(0)->get_pixel(0, 0); // fetch the first pixel from the first frame
	BOOST_CHECK_EQUAL (pixel.red, 160);
	BOOST_CHECK_EQUAL (pixel.green, 160);
	BOOST_CHECK_EQUAL (pixel.blue, 160);
	pixel = sum->get_frame(1)->get_pixel(0, 0); // fetch the first pixel from the second frame
	BOOST_CHECK_EQUAL (pixel.red, 255);
	BOOST_CHECK_EQUAL (pixel.green, 255);
	BOOST_CHECK_EQUAL (pixel.blue, 255);
	pixel = sum->get_frame(2)->get_pixel(0, 0); // fetch the first pixel from the third frame
	BOOST_CHECK_EQUAL (pixel.red, 160);
	BOOST_CHECK_EQUAL (pixel.green, 160);
	BOOST_CHECK_EQUAL (pixel.blue, 160);	
	pixel = sum->get_frame(3)->get_pixel(0, 0); // fetch the first pixel from the fourth frame
	BOOST_CHECK_EQUAL (pixel.red, 0);
	BOOST_CHECK_EQUAL (pixel.green, 0);
	BOOST_CHECK_EQUAL (pixel.blue, 0);	
//	std::cout << "####### The sum is the following:" << std::endl;
//	sum->dump(std::cout);	
}

BOOST_AUTO_TEST_CASE ( check_sequence_add_offset2 ) {
	init_color();
	std::cout << "################## Testing the addition of two sequences (with an offset, variation2)." << std::endl;
	fullcircle::RGB_t color1, pixel;
	color1.red = color1.green = color1.blue = 160;
	
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));
	fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty(new fullcircle::Frame(4,4));
	f1->fill_whole(color1);
	seq->add_frame(f1);
	seq->add_frame(f1);
	//	seq->dump(std::cout);
	
	fullcircle::Sequence::Ptr seq2(new fullcircle::Sequence(10,4,4));
    fullcircle::Frame::Ptr f2(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty2(new fullcircle::Frame(4,4));
	f2->fill_whole(color1);
	seq2->add_frame(f2);
	seq2->add_frame(f_empty2);
	//	seq2->dump(std::cout);
	
	fullcircle::Sequence::Ptr sum = seq->add(2, seq2);
	
    BOOST_CHECK_EQUAL (4, sum->size());
	pixel = sum->get_frame(0)->get_pixel(0, 0); // fetch the first pixel from the first frame
	BOOST_CHECK_EQUAL (pixel.red, 160);
	BOOST_CHECK_EQUAL (pixel.green, 160);
	BOOST_CHECK_EQUAL (pixel.blue, 160);
	pixel = sum->get_frame(1)->get_pixel(0, 0); // fetch the first pixel from the second frame
	BOOST_CHECK_EQUAL (pixel.red, 160);
	BOOST_CHECK_EQUAL (pixel.green, 160);
	BOOST_CHECK_EQUAL (pixel.blue, 160);
	pixel = sum->get_frame(2)->get_pixel(0, 0); // fetch the first pixel from the third frame
	BOOST_CHECK_EQUAL (pixel.red, 160);
	BOOST_CHECK_EQUAL (pixel.green, 160);
	BOOST_CHECK_EQUAL (pixel.blue, 160);	
	pixel = sum->get_frame(3)->get_pixel(0, 0); // fetch the first pixel from the fourth frame
	BOOST_CHECK_EQUAL (pixel.red, 0);
	BOOST_CHECK_EQUAL (pixel.green, 0);
	BOOST_CHECK_EQUAL (pixel.blue, 0);	
	//	std::cout << "####### The sum is the following:" << std::endl;
	//	sum->dump(std::cout);	
}

BOOST_AUTO_TEST_CASE ( check_sequence_add_offset3 ) {
	init_color();
	std::cout << "################## Testing the addition of two sequences (with an offset, variation3)." << std::endl;
	fullcircle::RGB_t color1;
	color1.red = color1.green = color1.blue = 160;
	
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));
	fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty(new fullcircle::Frame(4,4));
	f1->fill_whole(color1);
	seq->add_frame(f1);
	seq->add_frame(f1);
	//	seq->dump(std::cout);
	
	fullcircle::Sequence::Ptr seq2(new fullcircle::Sequence(10,4,4));
    fullcircle::Frame::Ptr f2(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty2(new fullcircle::Frame(4,4));
	f2->fill_whole(color1);
	seq2->add_frame(f2);
	seq2->add_frame(f_empty2);
	//	seq2->dump(std::cout);
	try {
		fullcircle::Sequence::Ptr sum = seq->add(3, seq2);
		BOOST_FAIL( "There was no Exception, even if the sequence was too short" );
	} catch (fullcircle::GenericException& ex) {
		//std::cout << "Caught exception: " << ex.what() << std::endl;
	}
}

BOOST_AUTO_TEST_CASE ( check_sequence_add_round ) {
	init_color();
	std::cout << "################## Testing the addition of two sequences (first sequence is used as a ringbuffer)." << std::endl;
	fullcircle::RGB_t color1;
	color1.red = color1.green = color1.blue = 160;
	fullcircle::RGB_t color2;
	color2.red = color2.green = 0; color2.blue = 255;
	
	/* Create a first sequence of 2 frames */
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));
	fullcircle::Frame::Ptr f10(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f11(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty(new fullcircle::Frame(4,4));
	f10->fill_whole(color1);
	f11->fill_whole(color2);
	seq->add_frame(f10);
	seq->add_frame(f11);
	//	seq->dump(std::cout);

		/* Create a second sequence of 3 frames */
	fullcircle::Sequence::Ptr seq2(new fullcircle::Sequence(10,4,4));
    fullcircle::Frame::Ptr f20(new fullcircle::Frame(4,4));
    fullcircle::Frame::Ptr f21(new fullcircle::Frame(4,4));
	fullcircle::Frame::Ptr f_empty2(new fullcircle::Frame(4,4));
	f20->fill_whole(color1);
	f21->fill_whole(color1);
	seq2->add_frame(f20);
	seq2->add_frame(f_empty2);
	seq2->add_frame(f21);
	
	//	seq2->dump(std::cout);
	fullcircle::Sequence::Ptr sum = seq->add(0, seq2);
}

//BOOST_AUTO_TEST_SUITE_END()
