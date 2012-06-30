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
#include <QApplication>
#include <libfullcircle/fontrenderer.hpp>

namespace bfs=boost::filesystem;

/***
 * We use a global fixture to initialize the sprites exactly once.
 * Remember, this must be done once per binary.
 */
struct GlobalSetup {
    GlobalSetup() 
      : has_loaded_sprites(false)
    { 
      if (! has_loaded_sprites) {
        std::cout << "Setup of QT sprites." << std::endl;
        Q_INIT_RESOURCE(sprites);
        has_loaded_sprites = true;
      }
    }
    ~GlobalSetup()  { std::cout << "global teardown" << std::endl; }
    bool has_loaded_sprites;
};

BOOST_GLOBAL_FIXTURE( GlobalSetup );


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

BOOST_AUTO_TEST_CASE ( check_font ) {
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(25,10,5));
	fullcircle::FontRenderer::Ptr fr(new fullcircle::FontRenderer(10, 5));
	fr->write_text(seq, 0, 0, "C3");
		
	seq->dump(std::cout);
}

BOOST_AUTO_TEST_CASE ( check_font_color ) {
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(25,11,5));
	fullcircle::FontRenderer::Ptr fr(new fullcircle::FontRenderer(11, 5));
	fullcircle::RGB_t blue;
	blue.red = blue.green = 0; blue.blue = 255;
	fr->write_text(seq, 0, 0, "MA", blue);
		
	/* only check some parts of the generated text ... */
	BOOST_CHECK_EQUAL(1, seq->size());
	BOOST_CHECK_EQUAL(11, seq->get_frame(0)->width());
	 /* vertical first line from the M */
	 fullcircle::RGB_t setcolor0=seq->get_frame(0)->get_pixel(1,0);
	 BOOST_CHECK_EQUAL(0, setcolor0.red);
	 BOOST_CHECK_EQUAL(0, setcolor0.green);
	 BOOST_CHECK_EQUAL(255, setcolor0.blue);
	setcolor0=seq->get_frame(0)->get_pixel(1,1);
	BOOST_CHECK_EQUAL(0, setcolor0.red);
	BOOST_CHECK_EQUAL(0, setcolor0.green);
	BOOST_CHECK_EQUAL(255, setcolor0.blue);
	setcolor0=seq->get_frame(0)->get_pixel(1,2);
	BOOST_CHECK_EQUAL(0, setcolor0.red);
	BOOST_CHECK_EQUAL(0, setcolor0.green);
	BOOST_CHECK_EQUAL(255, setcolor0.blue);
	setcolor0=seq->get_frame(0)->get_pixel(1,3);
	BOOST_CHECK_EQUAL(0, setcolor0.red);
	BOOST_CHECK_EQUAL(0, setcolor0.green);
	BOOST_CHECK_EQUAL(255, setcolor0.blue);
	setcolor0=seq->get_frame(0)->get_pixel(1,4);
	BOOST_CHECK_EQUAL(0, setcolor0.red);
	BOOST_CHECK_EQUAL(0, setcolor0.green);
	BOOST_CHECK_EQUAL(255, setcolor0.blue);
	/* last vertical line from the A */
	setcolor0=seq->get_frame(0)->get_pixel(10,0);
	BOOST_CHECK_EQUAL(0, setcolor0.red);
	BOOST_CHECK_EQUAL(0, setcolor0.green);
	BOOST_CHECK_EQUAL(255, setcolor0.blue);
	setcolor0=seq->get_frame(0)->get_pixel(10,1);
	BOOST_CHECK_EQUAL(0, setcolor0.red);
	BOOST_CHECK_EQUAL(0, setcolor0.green);
	BOOST_CHECK_EQUAL(255, setcolor0.blue);
	setcolor0=seq->get_frame(0)->get_pixel(10,2);
	BOOST_CHECK_EQUAL(0, setcolor0.red);
	BOOST_CHECK_EQUAL(0, setcolor0.green);
	BOOST_CHECK_EQUAL(255, setcolor0.blue);
	setcolor0=seq->get_frame(0)->get_pixel(10,3);
	BOOST_CHECK_EQUAL(0, setcolor0.red);
	BOOST_CHECK_EQUAL(0, setcolor0.green);
	BOOST_CHECK_EQUAL(255, setcolor0.blue);
	setcolor0=seq->get_frame(0)->get_pixel(10,4);
	BOOST_CHECK_EQUAL(0, setcolor0.red);
	BOOST_CHECK_EQUAL(0, setcolor0.green);
	BOOST_CHECK_EQUAL(255, setcolor0.blue);
		
	seq->dump(std::cout);
}

BOOST_AUTO_TEST_CASE ( check_font_offset ) {
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(25,15,8));
	fullcircle::FontRenderer::Ptr fr(new fullcircle::FontRenderer(15, 8));
	fr->write_text(seq, 1, 2, "C3");
	seq->dump(std::cout);
}

BOOST_AUTO_TEST_CASE ( check_font_height_exception ) {
	std::cout << "----- Waiting for Exception -------" << std::endl;
	  try {
		  fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(25,10,5));
		  fullcircle::FontRenderer::Ptr fr(new fullcircle::FontRenderer(10, 4));
    	  fr->write_text(seq, 0, 0, "C3");
		  BOOST_FAIL("No Exception found, because the screen is not heigh enough");
	  } catch (std::exception const & ex) {
		  
	  }	
	std::cout << "----- Exception was found -------" << std::endl;
}

BOOST_AUTO_TEST_CASE ( check_font_scrolling ) {
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(25,10,5));
	fullcircle::FontRenderer::Ptr fr(new fullcircle::FontRenderer(10, 5));
	fr->scroll_text(seq, 0, 0, "C3MA", 500);	
}

//BOOST_AUTO_TEST_SUITE_END()
