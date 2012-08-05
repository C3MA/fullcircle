#include <libfullcircle/common.hpp>
#include <libfullcircle/printer/printerinterface.hpp>
#include <libfullcircle/printer/badge_renderer.hpp>
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
      ("printerdevice,d", po::value<std::string>(), "the serial device of the printer")
      ("printertemplate,t", po::value<std::string>(), "the template file to use")
      ("color,c", po::value<std::string>(), "the color string")
			;

    std::ostringstream coss;
    coss << "configuration file (" << config_file << " by default).";
		po::options_description cmd("Command line options");
		cmd.add_options()
			("config", po::value<std::string>(), coss.str().c_str())
			("help,h", "produce help message")
			("version,v", "print version and exit")
      ("simulate,s", "do not print the badge - just dump it on the console.")
      ("number,n", po::value<std::string>(), "the interaction number to print")
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
			po::store(po::parse_config_file<char>(config_file.c_str(), 
            config_file_options, 
            true // ignore unknown options
          ), vm);
    }
		po::notify(vm);

    // Begin processing of commandline parameters.
    std::string device;
    std::string tmpl;
    std::string color;
    std::string number;

    if (vm.count("help")) {
      std::cout << cmdline_options << std::endl;
      return 1;
    }

    fullcircle::VersionInfo::Ptr version(new fullcircle::VersionInfo());
    if (vm.count("version")) {
      std::cout << "fullcircle version " << version->getVersion() << std::endl;
      return 0;
    }

    if (vm.count("printerdevice") != 1 ) {
      std::cerr << "You must specify the printer's device (-d <path>)." << std::endl;
      return 1;
    } else {
      device=vm["printerdevice"].as<std::string>();
    }

    if (vm.count("printertemplate") != 1 ) {
      std::cerr << "You must specify the file containing the badge template (-t <path>)." << std::endl;
      return 1;
    } else {
      tmpl=vm["printertemplate"].as<std::string>();
    }

    if (vm.count("color") != 1 ) {
      std::cerr << "You must specify the color (-c <color>)." << std::endl;
      return 1;
    } else {
      color=vm["color"].as<std::string>();
    }

    if (vm.count("number") != 1 ) {
      std::cerr << "You must specify the number (-n <number>)." << std::endl;
      return 1;
    } else {
      number=vm["number"].as<std::string>();
    }

    bfs::path devicefile(device);
    bfs::path tmplfile(tmpl);

    fullcircle::BadgeRenderer::Ptr badge(new fullcircle::BadgeRenderer(tmplfile));
    std::string document=badge->render(color, number);

    if (vm.count("simulate") == 0) {
      fullcircle::PrinterInterface::Ptr printer(
          new fullcircle::PrinterInterface(devicefile));
      printer->print(document);
      printer->cut_paper();
    } else {
      std::cout << "Simulating badge print (may contain command sequences)" << std::endl
        << "-------------------------------------------" << std::endl; 
      std::cout << std::endl << std::endl << document << std::endl;
      std::cout << "-------------------------------------------" << std::endl; 
    }

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

