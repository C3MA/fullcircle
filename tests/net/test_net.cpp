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
#include <sstream>
#include <testconfig.h>
#include <libfullcircle/net/envelope.hpp>
#include <libfullcircle/net/net_server.hpp>
#include <libfullcircle/net/net_client.hpp>
#include <libfullcircle/net/client_dispatcher.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <libfullcircle/sequence.pb.h>

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
  std::cout << "### Testing the envelope." << std::endl;
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
  std::cout << "### Testing a ping snip." << std::endl;
  fullcircle::Snip snip;
  snip.set_type(fullcircle::Snip::PING);
  fullcircle::Snip_PingSnip* ping=snip.mutable_ping_snip();
  ping->set_count(0);
  std::ostringstream oss;
  if (!snip.SerializeToOstream(&oss)) {
    BOOST_FAIL("ping snip serialization did not work.");
  }
  oss.flush();
  fullcircle::Envelope::Ptr env(new fullcircle::Envelope());
  env->set_body(oss.str());
  // simulate transfer over the network.
  char bytes[fullcircle::Envelope::header_length + 
    fullcircle::Envelope::max_body_length];
  size_t length=0;
  env->get_bytes(bytes, length);
  fullcircle::Envelope::Ptr env2(
      new fullcircle::Envelope(bytes, length));
  std::istringstream iss(env2->get_body());
  fullcircle::Snip snip2;
  if (!snip2.ParseFromIstream(&iss)) {
    BOOST_FAIL("Cannot load snip from input stream.");
  }
  std::cout << "Debug: " << snip2.DebugString() << std::endl;
  fullcircle::Snip_PingSnip ping2=snip2.ping_snip();
  BOOST_CHECK_EQUAL (0, ping2.count());
}

BOOST_AUTO_TEST_CASE ( check_ping_online ) {
  std::cout << "1. preparing server instance." << std::endl;
  boost::asio::io_service server_io_service;
  boost::asio::ip::tcp::endpoint server_endpoint(
      boost::asio::ip::tcp::v4(), TEST_SERVER_PORT);
  fullcircle::NetServer::Ptr server = fullcircle::NetServer::create(
      server_io_service, server_endpoint);
  server->run();

  std::cout << "##### Waiting" << std::endl;
  boost::this_thread::sleep( boost::posix_time::seconds(1) );

  std::cout << "2. preparing client instance." << std::endl;
  boost::asio::io_service client_io_service;
  boost::asio::ip::tcp::resolver resolver(client_io_service);
  std::ostringstream oss;
  oss << TEST_SERVER_PORT;
  boost::asio::ip::tcp::resolver::query query(
      "localhost", oss.str());
  boost::asio::ip::tcp::resolver::iterator iterator = 
    resolver.resolve(query);
  fullcircle::NetClient::Ptr client(
    new fullcircle::NetClient(client_io_service, iterator));
  fullcircle::ClientDispatcher::Ptr c_disp(
      new fullcircle::ClientDispatcher(client));
  client->do_on_envelope(
      boost::bind(&fullcircle::ClientDispatcher::handle_envelope, 
        c_disp, _1));
  client->run();

  std::cout << "##### Waiting" << std::endl;
  boost::this_thread::sleep( boost::posix_time::seconds(1) );

  std::cout << "3. Attempting to send a ping message." << std::endl;
  fullcircle::Snip snip;
  snip.set_type(fullcircle::Snip::PING);
  fullcircle::Snip_PingSnip* ping=snip.mutable_ping_snip();
  ping->set_count(0);
  std::ostringstream oss2;
  if (!snip.SerializeToOstream(&oss2)) {
    BOOST_FAIL("ping snip serialization did not work.");
  }
  oss2.flush();
  fullcircle::Envelope::Ptr env(new fullcircle::Envelope());
  env->set_body(oss2.str());
  client->write(env);
  ping->set_count(1);
  oss2.clear();
  oss2.str("");
  if (!snip.SerializeToOstream(&oss2)) {
    BOOST_FAIL("ping snip serialization did not work.");
  }
  oss2.flush();
  fullcircle::Envelope::Ptr env2(new fullcircle::Envelope());
  env2->set_body(oss2.str());
  client->write(env2);


  std::cout << "##### Waiting" << std::endl;
  boost::this_thread::sleep( boost::posix_time::seconds(1) );
  std::cout << "4. Initiating shutdown." << std::endl;
  client->shutdown();
  server->shutdown();
  std::cout << "EOT" << std::endl;
}
