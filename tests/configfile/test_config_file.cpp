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

#define BOOST_TEST_MODULE config_test
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include <testconfig.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/cstdint.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
namespace po = boost::program_options;
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

BOOST_AUTO_TEST_CASE ( config_file ) {
  std::cout << "### Testing config file." << std::endl;

  boost::filesystem::path config_file(TEST_CONFIG_FILE);
  std::ofstream cfg_file;
  cfg_file.open(config_file.c_str());
  cfg_file << "width = 16" << std::endl;
  cfg_file << "height = 9" << std::endl;
  cfg_file << "fps = 15" << std::endl;
  cfg_file.close();

  std::cout << "Using test fixture file " << config_file << std::endl;

  po::options_description generic("config file options");
  generic.add_options()
    ("width,w", po::value<std::string>(), "the width of the sequence to be generated.")
    ("height,h", po::value<std::string>(), "the height of the sequence to be generated.")
    ("fps,f", po::value<std::string>(), "the frames per second of the sequence to be generated.")
    ;

  po::options_description cmdline_options("testing options");
  cmdline_options.add(generic);
  std::cout << cmdline_options << std::endl;

  po::variables_map vm;
  if ( boost::filesystem::exists(config_file) ) {
    po::store(po::parse_config_file<char>(config_file.c_str(), cmdline_options), vm);
  }
  po::notify(vm);

  if (vm.count("width") == 1 ) {
    uint16_t width;
    std::istringstream converter(vm["width"].as<std::string>());
    if ( !( converter >> width)) {
      BOOST_FAIL("Cannot convert width to an integer. ");
    }
    BOOST_REQUIRE(width == 16);
  } else {
    BOOST_FAIL("Cannot find width command line parameter.");
  }
  if (vm.count("height") == 1 ) {
    BOOST_REQUIRE(boost::algorithm::iequals(vm["height"].as<std::string>(), "9"));
  } else {
    BOOST_FAIL("Cannot find height command line parameter.");
  }
  if (vm.count("fps") == 1 ) {
    BOOST_REQUIRE(boost::algorithm::iequals(vm["fps"].as<std::string>(), "15"));
  } else {
    BOOST_FAIL("Cannot find fps command line parameter.");
  }
}


//BOOST_AUTO_TEST_SUITE_END()
