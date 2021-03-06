#include <libfullcircle/common.hpp>
#include <libfullcircle/redis.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>

namespace po = boost::program_options;
namespace bfs=boost::filesystem;


int main(int argc, char *argv[1]) {
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
      ("redisserver,s", po::value<std::string>(), "address of the redis server")
      ("redisport,p", po::value<std::string>(), "port of the redis server")
			;

    std::ostringstream coss;
    coss << "configuration file (" << config_file << " by default).";
		po::options_description cmd("Command line options");
		cmd.add_options()
			("config", po::value<std::string>(), coss.str().c_str())
			("help,h", "produce help message")
			("version,v", "print version and exit")
			;

		std::ostringstream oss;
		oss << "Usage: " << argv[0] << " ACTION [additional options]";
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

    if (vm.count("redisserver") != 1 ) {
      std::cerr << "You must specify the redisserver (-s <server>)." << std::endl;
      return 1;
    } else {
      server=vm["redisserver"].as<std::string>();
    }

    if (vm.count("redisport") != 1 ) {
      std::cerr << "Warning: no port specified. Assuming REDIS default port 6379." << std::endl;
      port = 6379;
    } else {
      std::istringstream converter(vm["redisport"].as<std::string>());
      if ( !( converter >> port)) {
        std::cerr << "Cannot convert port to an integer. " << std::endl;
        return 1;
      }
    }

    fullcircle::Redis::Ptr r(new fullcircle::Redis(server, port));
    std::cout << r->get_unique_number() << std::endl;

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

