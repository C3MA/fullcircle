#ifndef FULLCIRCLE__CLIENT_HPP_
#define FULLCIRCLE__CLIENT_HPP_

#include <libfullcircle/net/net_client.hpp>
#include <libfullcircle/net/client_dispatcher.hpp>

namespace fullcircle {
	enum clientState_t {
		IDLE = 0,
		WAIT_FOR_ACK,
		WAIT_FOR_START,
		PLAYING,
		ERROR
	};
	const char* stateNames[] = { "IDLE", "WAIT_FOR_ACK", "WAIT_FOR_START", "PLAYING", "ERROR" };

	/* The client class is a base class for implementing your own clients.
	 */
	class Client {
		public:
			Client(boost::asio::io_service *client_io_service, boost::asio::ip::tcp::resolver::iterator *iterator)
				: _state(IDLE),
					_netClient(
						new fullcircle::NetClient(*client_io_service, *iterator)),
					_dispatcher(
						new fullcircle::ClientDispatcher(_netClient)),
					_timer(*client_io_service)
			{
				_netClient->do_on_envelope(
						boost::bind(&fullcircle::ClientDispatcher::handle_envelope, 
							_dispatcher, _1));
				// link all protobuf types to cpp-functions
				_netClient->do_on_error(boost::bind(&fullcircle::Client::do_on_error, this, _1));
				_dispatcher->do_on_ack(boost::bind(&fullcircle::Client::do_on_ack, this));
				_dispatcher->do_on_nack(boost::bind(&fullcircle::Client::do_on_nack, this));
				_dispatcher->do_on_start(boost::bind(&fullcircle::Client::do_on_start, this));
			}

			~Client()
			{
    		_netClient->shutdown();
			}

			/* The following functions have to be implemented. They may be empty and should contain tasks,
			 * that have to be executed when entering the corresponding state.
			 */
			virtual void idle() = 0;
			virtual void ack() = 0;
			virtual void nack() = 0;
			virtual void start() = 0;

			/* The error handler has the following default implementation, logging the error and leaving the client.
			 */
			virtual void error(std::string error_msg = "")
			{
				std::cout << "Server connection failed - " << error_msg << std::endl;
				exit(1);
			}

			/* For the following functions it should not be nessecary to reimplement them as the contain the basic
			 * message handling, timeouts and state transitions.
			 */
			void do_on_ack()
			{
				if ( _state != WAIT_FOR_ACK )
				{
					_state = ERROR;
					std::stringstream ss;
					ss << "Unexpected ACK received while in state " << stateNames[_state];
					error(ss.str());
					return;
				}

				_timer.cancel();
				_state = WAIT_FOR_START;
				ack();

				_timer.expires_from_now(boost::posix_time::seconds(15));
				_timer.async_wait(boost::bind(&fullcircle::Client::do_on_timeout, this, _1));
			}

			void do_on_nack()
			{
				if ( _state != WAIT_FOR_ACK )
				{
					_state = ERROR;
					std::stringstream ss;
					ss << "Unexpected NACK received while in state " << stateNames[_state];
					error(ss.str());
					return;
				}

				_timer.cancel();
				_state = ERROR;
				nack();
			}

			void do_on_start()
			{
				if ( _state != WAIT_FOR_START )
				{
					_state = ERROR;
					std::stringstream ss;
					ss << "Unexpected START received while in state " << stateNames[_state];
					error(ss.str());
					return;
				}

				_timer.cancel();
				_state = PLAYING;
				start();
			}

			void do_on_error(std::string error_msg)
			{
				_state = ERROR;
				error(error_msg);
			}

			void do_on_timeout(const boost::system::error_code& e)
			{
				// timer canceled, nothing to do here
				if ( e == boost::asio::error::operation_aborted )
					return;

				_state = ERROR;
				if ( !e )
					error("Timeout!");
				else
					error("Unkown timeout error!");
			}

			void run()
			{
				_netClient->run();
				idle();
			}

			void send_request(std::string color, int id, fullcircle::BinarySequenceMetadata* meta)
			{
				_dispatcher->send_request(color, id, meta);

				_state = WAIT_FOR_ACK;
				_timer.expires_from_now(boost::posix_time::seconds(5));
				_timer.async_wait(boost::bind(&fullcircle::Client::do_on_timeout, this, _1));
			}

		protected:
			clientState_t _state;
			fullcircle::NetClient::Ptr _netClient;
			fullcircle::ClientDispatcher::Ptr _dispatcher;
			boost::asio::deadline_timer _timer;
	};
}

#endif // FULLCIRCLE__CLIENT_HPP_
