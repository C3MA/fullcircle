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

#define BOOST_TEST_MODULE net_test
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include <testconfig.h>
#include <libfullcircle/net/envelope.hpp>
#include <boost/algorithm/string/predicate.hpp>

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

BOOST_AUTO_TEST_CASE ( check_envelope ) {
  fullcircle::Envelope::Ptr env(new fullcircle::Envelope());
  std::string payload("Payload! Wohooo!");
  env->set_body(payload);
  std::cout << "Created envelope: " << env->str() << std::endl;
  // 1. Is the length of the payload calculated correctly?
  BOOST_CHECK_EQUAL(env->get_body_length(), payload.size());
  // 2. Retrieve the payload and make sure it wasn't altered.
  std::string retrieved_payload(env->get_body());
  if (! boost::equals(payload, retrieved_payload)) {
    BOOST_FAIL( "Envelope does not encode payload correctly");
    std::cout << "Payload was >" << payload << "<" << std::endl;
    std::cout << "Retrieved was >" << retrieved_payload << "<" << std::endl;
  }
  std::cout << "Retrieved payload: " << retrieved_payload << std::endl;
  // 3. Simulate the transfer over the network.
  char bytes[fullcircle::Envelope::header_length + 
    fullcircle::Envelope::max_body_length];
  size_t length=0;
  env->get_bytes(bytes, length);
  std::cout << "Got " << length << " raw bytes: " << bytes << std::endl;
  fullcircle::Envelope::Ptr env2(
      new fullcircle::Envelope(bytes, length));
  if (! boost::equals(payload, env2->get_body())) {
    BOOST_FAIL( "Envelope not correctly reconstructed");
    std::cout << "Payload was >" << payload << "<" << std::endl;
    std::cout << "Retrieved was >" << env2->get_body() << "<" << std::endl;
  }
}

BOOST_AUTO_TEST_CASE ( check_ping ) {

}

