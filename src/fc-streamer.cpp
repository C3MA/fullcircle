#include <libfullcircle/common.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/net/client.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <fstream>

namespace po = boost::program_options;
namespace bfs=boost::filesystem;

/* The StreamingClient is a reference implementation of a client for fullcircle.
 */
namespace fullcircle {
  class StreamingClient : public Client {
    public:
      StreamingClient(boost::asio::io_service *client_io_service, boost::asio::ip::tcp::resolver::iterator *iterator, std::string source)
        : Client(client_io_service, iterator),
        _file(source)
    {
    }

      ~StreamingClient()
      {
      }

      void idle()
      {
        // read fcs file in order to get the meta information
        std::cout << "Reading " << _file.c_str() << std::endl;
        std::fstream input(_file.c_str(), std::ios::in | std::ios::binary);
        _sequence.reset(new fullcircle::Sequence(input));
        input.close();

        // fill meta information with content of the fcs-file
        fullcircle::BinarySequenceMetadata* meta = new fullcircle::BinarySequenceMetadata();
        meta->set_frames_per_second(_sequence->fps());
        meta->set_width(_sequence->width());
        meta->set_height(_sequence->height());
        meta->set_generator_name(_sequence->generator_name());
        meta->set_generator_version(_sequence->generator_version());

        send_request("blue", 14, meta);
      }

      virtual void ack()
      {
        std::cout << "Ack" << std::endl;
      }

      virtual void nack()
      {
      }

      virtual void start()
      {
        _ifs = 1000/_sequence->fps();
        _frameId = 0;
        _timer.expires_from_now(boost::posix_time::milliseconds(_ifs));
        _timer.async_wait(boost::bind(&fullcircle::StreamingClient::send_frame, this));
      }

      void send_frame()
      {
        if ( _frameId < _sequence->size() )
        {
          fullcircle::Frame::Ptr frame = _sequence->get_frame(_frameId++);
          _dispatcher->send_frame(frame);

          _timer.expires_from_now(boost::posix_time::milliseconds(_ifs));
          _timer.async_wait(boost::bind(&fullcircle::StreamingClient::send_frame, this));
        } else {
          _dispatcher->send_eos();
          _state = IDLE;
          exit(0);
        }
      }

    private:
      std::string _file;
      fullcircle::Sequence::Ptr _sequence;
      uint32_t _frameId;
      double _ifs;
  };
}

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

    fullcircle::StreamingClient client(&client_io_service, &iterator, srcfile);
    client.run();

    // enter the asio event loop
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

