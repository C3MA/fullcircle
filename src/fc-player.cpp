#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/multi_array.hpp>
#include <libfullcircle/common.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/scheduler.hpp>
#include <libfullcircle/dmxclient.hpp>

namespace po = boost::program_options;
namespace pt = boost::property_tree;

int main (int argc, char* argv[]) {
	//default values
  boost::filesystem::path config_file;
  char* homedir = getenv("HOME");
  if (homedir != NULL) {
    config_file = config_file / homedir / ".fullcirclerc";
  } else {
    config_file = config_file / "etc" / "fullcirclerc";
  }
	int universe = 1;
	bool debug = false;

	try {
		po::options_description generic("Generic options");
		generic.add_options()
			("input,i", po::value<std::string>(), "the directory containing sequence files")
			("priority,p", po::value<std::string>(), "the directory containing priority sequence files")
			("fallback,f", po::value<std::string>(), "the directory containing fallback sequence files")
			("output,o", po::value<std::string>(), "the directory played sequence files are moved to")
			("port", po::value<int>(), "the port to listen for network streams")
			("universe,u", po::value<int>(), "the DMX universe used for output")
			("debug,d", "print additional info")
			;

		po::options_description cmd("Command line options");
		cmd.add_options()
			("config,c", po::value<std::string>(), "configuration file")
			("help,h", "produce help message")
			("version,v", "print version and exit")
			;

		std::ostringstream oss;
		oss << "Usage: " << argv[0] << " [options]";
		po::options_description cmdline_options(oss.str());
		cmdline_options.add(generic).add(cmd);

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
		po::notify(vm);

		// Begin processing of commandline parameters.
		if (vm.count("help")) {
			std::cout << cmdline_options << std::endl;
			return 1;
		}

		if (vm.count("version")) {
			fullcircle::VersionInfo::Ptr version(new fullcircle::VersionInfo());
			std::cout << "fc-player version " << version->getVersion() << std::endl;
			return 0;
		}

		if ( vm.count("debug") ) {
			debug = true;
		}

		if (vm.count("config")) {
			boost::filesystem::path temp(vm["config"].as<std::string>());
			if ( boost::filesystem::exists(temp) )
				config_file = temp;
			else {
				std::cerr << "Configuration file " << vm["config"].as<std::string>() << " not found!" << std::endl;
				return 1;
			}
		}

		std::string srcdir, priodir, falldir, dstdir;
		int port = 0;
		fullcircle::Scheduler::Ptr scheduler(new fullcircle::Scheduler());

		std::map< int, std::map<int, int> > map;
		if ( boost::filesystem::exists(config_file) )
		{
			try {
				pt::ptree tree;

				pt::read_ini(config_file.c_str(), tree);

				BOOST_FOREACH(pt::ptree::value_type &v, tree)
				{
					std::stringstream ss(v.first);

					int y;
					if ( ss >> y )
					{
						BOOST_FOREACH(pt::ptree::value_type &addr, v.second)
						{
							std::stringstream str(addr.first);

							debug && std::cout << "Parsing row " << y << std::endl;


							int x;
							if ( str >> x )
								map[x][y] = addr.second.get_value<int>();
						}
					} else if ( v.first.compare("input") == 0 )
						srcdir = v.second.get_value<std::string>();
					else if ( v.first.compare("output") == 0 )
						dstdir = v.second.get_value<std::string>();
					else if ( v.first.compare("priority") == 0 )
						priodir = v.second.get_value<std::string>();
					else if ( v.first.compare("fallback") == 0 )
						falldir = v.second.get_value<std::string>();
					else if ( v.first.compare("port") == 0 )
						port = v.second.get_value<int>();
					else if ( v.first.compare("universe") == 0 )
						universe = v.second.get_value<int>();
				}
				//std::cout << "map: " << map.size() << std::endl;
			} catch (std::exception& e) {
				std::cerr << "Parsing failed: " << e.what() << std::endl;
				return 1;
			}
			if ( debug )
			{
				for ( unsigned int y = 0; y < map.size(); y++ )
				{
					std::cout << y << ": ";
					for ( unsigned int x = 0; x < map[y].size(); x++ )
						std::cout << map[x][y] << " ";
					std::cout << std::endl;
				}
			}
		}

		if ( vm.count("input") )
		{
			srcdir=vm["input"].as<std::string>();
		}

		if ( srcdir.empty()) {
			std::cerr << "You must specify a sequence directory." << std::endl;
			return 1;
		} else {
			if ( boost::filesystem::exists(srcdir) )
			{
				scheduler->setInput(srcdir);
				std::cout << "Using " << srcdir << " as source for sequence files" << std::endl;
			} else {
				std::cerr << srcdir << " does not exists!" << std::endl;
				return 1;
			}
		}

		if (vm.count("priority")) {
			priodir=vm["priority"].as<std::string>();
		}

		if ( !priodir.empty() )
		{
			if ( ! boost::filesystem::exists(priodir) )
				std::cerr << priodir << " does not exists!" << std::endl;
			else
			{
				std::cout << "Using " << priodir << " as source for priority sequence files" << std::endl;
				scheduler->setPriority(priodir);
			}
		}

		if (vm.count("fallback")) {
			falldir=vm["fallback"].as<std::string>();
		}

		if ( !falldir.empty() )
		{
			if ( ! boost::filesystem::exists(falldir) )
				std::cerr << falldir << " does not exists!" << std::endl;
			else
			{
				std::cout << "Using " << falldir << " as source for fallback sequence files" << std::endl;
				scheduler->setFallback(falldir);
			}
		}

		if ( vm.count("output") )
			dstdir=vm["output"].as<std::string>();

		if ( dstdir.empty() ) {
			std::cerr << "You must specify an output directory." << std::endl;
			return 1;
		} else {
			if ( boost::filesystem::exists(dstdir) )
			{
				scheduler->setOutput(dstdir);
				std::cout << "Using " << dstdir << " as destination for played sequence files" << std::endl;
			} else {
				std::cerr << dstdir << " does not exists!" << std::endl;
				return 1;
			}
		}

		if ( vm.count("port") )
			port = vm["port"].as<int>();

		if ( port != 0 )
		{
			std::cout << "Listening on port " << port << " for streams" << std::endl;
			scheduler->setPort(port);
		}

		if (vm.count("universe")) {
			universe=vm["universe"].as<int>();
			std::cout << "Using universe " << universe << std::endl;
		}

		fullcircle::DmxClient client(scheduler);
		client.setUniverse(universe);
		client.setMapping(&map);
		if ( debug )
		{
			client.setDebug(true);
			scheduler->setDebug(true);
		}
		client.start();

		std::string in;
		std::cin >> in;
		std::cout << in << std::endl;

		client.stop();
	} catch(std::exception& e) {
		std::cerr << "error: " << e.what() << std::endl;
		return 1;
	} catch(...) {
		std::cerr << "Exception of unknown type!" << std::endl;
		return 1;
	}
	return 0;
}


