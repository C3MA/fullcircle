#include <libfullcircle/common.hpp>
#include <libfullcircle/net/net_client.hpp>
#include <libfullcircle/net/client_dispatcher.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

namespace po = boost::program_options;
namespace pt = boost::posix_time;
namespace bfs=boost::filesystem;

std::map<uint16_t, pt::ptime> ping_send_time;

static void pong_receiver(fullcircle::Snip_PongSnip pong) {
  pt::ptime recv_time(pt::microsec_clock::local_time());
  pt::time_duration roundtrip = recv_time - ping_send_time[pong.count()];
  std::cout << "Ping " << pong.count() << ": " 
    << roundtrip.total_microseconds() << " µs." << std::endl;
  ping_send_time.erase(pong.count());
}

static bool shutdown_toggle = false;
static void net_error_receiver(std::string error_msg) {
  std::cout << "Server connection failed - " << error_msg << std::endl;
  shutdown_toggle = true;
}

int main (int argc, char const* argv[]) {
  std::cout << "FullCircle Ping Utility" << std::endl;
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

    boost::asio::io_service client_io_service;
    boost::asio::ip::tcp::resolver resolver(client_io_service);
    std::ostringstream oss2;
    oss2 << port;
    boost::asio::ip::tcp::resolver::query query(
        server, oss2.str());
    boost::asio::ip::tcp::resolver::iterator iterator = 
      resolver.resolve(query);
    fullcircle::NetClient::Ptr client(
        new fullcircle::NetClient(client_io_service, iterator));
    fullcircle::ClientDispatcher::Ptr c_disp(
        new fullcircle::ClientDispatcher(client));
    client->do_on_envelope(
        boost::bind(&fullcircle::ClientDispatcher::handle_envelope, 
          c_disp, _1));
    client->do_on_error(&net_error_receiver);
    c_disp->do_on_pong(&pong_receiver);
    client->run();
    
    uint16_t seq_id=0;
    while(! shutdown_toggle) {
      pt::ptime send_time(pt::microsec_clock::local_time());
      ping_send_time.insert ( 
          std::pair<uint16_t,pt::ptime>(seq_id, send_time));
      c_disp->send_ping(seq_id++);
      boost::this_thread::sleep( boost::posix_time::seconds(1) );
    }

    client->shutdown();
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

