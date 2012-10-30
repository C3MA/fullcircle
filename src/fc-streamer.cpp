#include <libfullcircle/common.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/net/net_client.hpp>
#include <libfullcircle/net/client_dispatcher.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <fstream>

namespace po = boost::program_options;
namespace bfs=boost::filesystem;

namespace fullcircle {
	enum clientState_t {
		IDLE = 0,
		WAIT_FOR_ACK,
		WAIT_FOR_START,
		ERROR
	};

	class Client {
		public:
			Client(boost::asio::io_service *client_io_service, boost::asio::ip::tcp::resolver::iterator *iterator, std::string source)
				: _state(IDLE),
					_netClient(
							new fullcircle::NetClient(*client_io_service, *iterator)),
					_dispatcher(
							new fullcircle::ClientDispatcher(_netClient)),
					_file(source)
			{
				_netClient->do_on_envelope(
						boost::bind(&fullcircle::ClientDispatcher::handle_envelope, 
							_dispatcher, _1));
				// link all protobuf types to cpp-functions
				_netClient->do_on_error(boost::bind(&fullcircle::Client::do_on_error, this, _1));
				_dispatcher->do_on_ack(boost::bind(&fullcircle::Client::do_on_ack, this));
				_dispatcher->do_on_nack(boost::bind(&fullcircle::Client::do_on_nack, this));
				_dispatcher->do_on_start(boost::bind(&fullcircle::Client::do_on_start, this));
				_netClient->run();

				idle();
			}

			~Client()
			{
    		_netClient->shutdown();
			}

			virtual void do_on_ack()
			{
				if ( _state != WAIT_FOR_ACK )
				{
					_state = ERROR;
					return;
				}

				_state = WAIT_FOR_START;
			};

			virtual void do_on_nack()
			{
				if ( _state != WAIT_FOR_ACK )
				{
					_state = ERROR;
					return;
				}

				std::cout << "Server responded with NACK. Aborting." << std::endl;
				_state = ERROR;
				exit(1);
			};

			virtual void do_on_start()
			{
				if ( _state != WAIT_FOR_START )
				{
					_state = ERROR;
					return;
				}

				std::cout << "Reading " << _file.c_str() << std::endl;
				std::fstream input(_file.c_str(), std::ios::in | std::ios::binary);
				fullcircle::Sequence::Ptr loaded_seq(new fullcircle::Sequence(input));
				input.close();

				double ifs = 1000/loaded_seq->fps();
				for ( uint32_t frameId = 0; frameId < loaded_seq->size(); frameId++ ) {
					std::cout << frameId << std::endl;
					fullcircle::Frame::Ptr frame = loaded_seq->get_frame(frameId);
					_dispatcher->send_frame(frame);

					usleep(ifs*1000);
				}

				_state = IDLE;
				exit(0);
			}

			virtual void do_on_error(std::string error_msg)
			{
				std::cout << "Server connection failed - " << error_msg << std::endl;
				exit(1);
			}

			void idle() {
				if ( _state != IDLE )
				{
					_state = ERROR;
					return;
				}

				// read fcs file in order to get the meta information
				std::cout << "Reading " << _file.c_str() << std::endl;
                                std::fstream input(_file.c_str(), std::ios::in | std::ios::binary);
                                fullcircle::Sequence::Ptr loaded_seq(new fullcircle::Sequence(input));
                                input.close();

				// fill meta information with content of the fcs-file
				fullcircle::BinarySequenceMetadata* meta = new fullcircle::BinarySequenceMetadata();
				meta->set_frames_per_second(loaded_seq->fps());
				meta->set_width(loaded_seq->width());
				meta->set_height(loaded_seq->height());
				meta->set_generator_name(loaded_seq->generator_name());
				meta->set_generator_version(loaded_seq->generator_version());

				// ask the server for a timeslot
				_dispatcher->send_request("blue", 14, meta);

				_state = WAIT_FOR_ACK;
			}

		private:
			clientState_t _state;
			fullcircle::NetClient::Ptr _netClient;
			fullcircle::ClientDispatcher::Ptr _dispatcher;
			std::string _file;
	};
}

/*static void pong_receiver(fullcircle::Snip_PongSnip pong) {
  pt::ptime recv_time(pt::microsec_clock::local_time());
  pt::time_duration roundtrip = recv_time - ping_send_time[pong.count()];
  std::cout << "Ping " << pong.count() << ": " 
  << roundtrip.total_microseconds() << " µs." << std::endl;
  ping_send_time.erase(pong.count());
  }*/

//static bool shutdown_toggle = false;
/*static void net_error_receiver(std::string error_msg) {
  std::cout << "Server connection failed - " << error_msg << std::endl;
  exit(1);
}*/

int main (int argc, char const* argv[]) {
  std::cout << "FullCircle Sequence Streamer" << std::endl;
  // default config file path
  boost::filesystem::path config_file;
  char* homedir = getenv("HOME");
  if (homedir != NULL) {
    config_file = config_file / homedir / ".fullcirclerc";
  } else {
    config_file = config_file / "etc" / "fullcirclerc";
  }


  try {
    po::options_description generic("Generic options (config file and command line)");
    generic.add_options()
      ("port,p", po::value<std::string>(), "the width of the sequence to be generated.")
      ("server,s", po::value<std::string>(), "the height of the sequence to be generated.")
      ;
    std::ostringstream coss;
    coss << "configuration file (" << config_file << " by default).";
    po::options_description cmd("Command line options");
    cmd.add_options()
      ("config", po::value<std::string>(), coss.str().c_str())
      ("input,i", po::value<std::string>(), "input sequence")
      ("help,?", "produce help message")
      ("version,v", "print version and exit")
      ;
    std::ostringstream oss;
    oss << "Usage: " << argv[0] << " -s <IP> -p <PORT> ...";
    po::options_description cmdline_options(oss.str());
    cmdline_options.add(generic).add(cmd);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
    po::notify(vm);

    // Load additional config file settings.
    if (vm.count("config")) {
      boost::filesystem::path temp(vm["config"].as<std::string>());
      if ( boost::filesystem::exists(temp) )
        config_file = vm["config"].as<std::string>();
      else {
        std::cerr << "Configuration file " << vm["config"].as<std::string>() << " not found!" << std::endl;
        return 1;
      }
    }

    po::options_description config_file_options;
    config_file_options.add(generic);
    boost::filesystem::path config(config_file);
    if ( boost::filesystem::exists(config) ) {
      po::store(po::parse_config_file<char>(config_file.c_str(), config_file_options, true), vm);
    }
    po::notify(vm);

    // Begin processing of commandline parameters.
    std::string server;
    std::string srcfile;
    uint16_t port;

    if (vm.count("help")) {
      std::cout << cmdline_options << std::endl;
      return 1;
    }

    fullcircle::VersionInfo::Ptr version(new fullcircle::VersionInfo());
    if (vm.count("version")) {
      std::cout << "fullcircle version " << version->getVersion() << std::endl;
      return 0;
    }

    if (vm.count("server") != 1 ) {
      std::cerr << "You must specify a server (-s <HOST/IP>)." << std::endl;
      return 1;
    } else {
      server=vm["server"].as<std::string>();
    }

    if (vm.count("port") != 1 ) {
      std::cerr << "You must specify a port. " << std::endl;
      return 1;
    } else {
      std::istringstream converter(vm["port"].as<std::string>());
      if ( !( converter >> port)) {
        std::cerr << "Cannot convert port to an integer. " << std::endl;
        return 1;
      }
    }

    if (vm.count("input") != 1 ) {
      std::cerr << "You mus specify an input file." << std::endl;
      return 1;
    } else {
      srcfile = vm["input"].as<std::string>();
    }

		boost::asio::io_service client_io_service;
		boost::asio::ip::tcp::resolver resolver(client_io_service);
		std::ostringstream oss2;
		oss2 << port;
		boost::asio::ip::tcp::resolver::query query(
				server, oss2.str());
		boost::asio::ip::tcp::resolver::iterator iterator = 
			resolver.resolve(query);
		
		fullcircle::Client client(&client_io_service, &iterator, srcfile);
    //TODO: wait for ack from the server

		//client.do_on_start();

		client_io_service.run();

  } catch (fullcircle::GenericException& ex) {
    std::cout << "Caught exception: " << ex.what() << std::endl;
    exit(1);
  } catch(std::exception& e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "Exception of unknown type!" << std::endl;
    return 1;
  }

  return 0;
}

