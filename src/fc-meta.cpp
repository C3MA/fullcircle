#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/filesystem.hpp>
#include <libfullcircle/common.hpp>
#include <libfullcircle/sequence.hpp>

namespace po = boost::program_options;

int main(int argc, char const* argv[])
{
  //std::cout << "FullCircle Sequence Streamer" << std::endl;
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
    oss << "Usage: " << argv[0] << " -i <INPUT> ...";
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

		std::string srcfile;

    po::options_description config_file_options;
    config_file_options.add(generic);
    boost::filesystem::path config(config_file);
    if ( boost::filesystem::exists(config) ) {
      po::store(po::parse_config_file<char>(config_file.c_str(), config_file_options, true), vm);
    }
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << cmdline_options << std::endl;
      return 1;
    }

    fullcircle::VersionInfo::Ptr version(new fullcircle::VersionInfo());
    if (vm.count("version")) {
      std::cout << "fullcircle version " << version->getVersion() << std::endl;
      return 0;
    }

    if (vm.count("input") != 1 ) {
      std::cerr << "You must specify an input file." << std::endl;
      return 1;
    } else {
      srcfile = vm["input"].as<std::string>();
    }

		std::fstream input(srcfile.c_str(), std::ios::in | std::ios::binary);
		fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(input));
		input.close();

		std::cout << "(1) fps: " << seq->fps() << std::endl;
		std::cout << "(2) width: " << seq->width() << std::endl;
		std::cout << "(3) height: " << seq->height() << std::endl;
		std::cout << "(4) generator name: " << seq->generator_name() << std::endl;
		std::cout << "(5) generator version: " << seq->generator_version() << std::endl;
		std::cout << "Which field do you want to edit?";
		int userinput;
		std::cin >> userinput;
		std::cout << "userinput: " << userinput << std::endl;
		switch (userinput)
		{
			case 1:
				{
				std::cout << "Please enter the new FPS: ";
				int fps;
				std::cin >> fps;
				std::cout << "Setting new FPS: " << fps;
				fullcircle::Sequence::Ptr sq(new fullcircle::Sequence(fps, seq->width(), seq->height()));
				sq->add_sequence(seq);
				std::fstream output(srcfile.c_str(), std::ios::out | std::ios::binary);
				sq->save(output, seq->generator_name(), seq->generator_version());
				}
				break;
			case 2:
				{
				std::cout << "Please enter the new width: ";
				int width;
				std::cin >> width;
				std::cout << "Setting new width: " << width;
				fullcircle::Sequence::Ptr sq(new fullcircle::Sequence(seq->fps(), width, seq->height()));
				sq->add_sequence(seq);
				std::fstream output(srcfile.c_str(), std::ios::out | std::ios::binary);
				sq->save(output, seq->generator_name(), seq->generator_version());
				}
				break;
			case 3:
				{
				std::cout << "Please enter the new height: ";
				int height;
				std::cin >> height;
				std::cout << "Setting new height: " << height;
				fullcircle::Sequence::Ptr sq(new fullcircle::Sequence(seq->fps(), seq->width(), height));
				sq->add_sequence(seq);
				std::fstream output(srcfile.c_str(), std::ios::out | std::ios::binary);
				sq->save(output, seq->generator_name(), seq->generator_version());
				}
				break;
			case 4:
				{
				std::cout << "Please enter the new generator name: ";
				std::string gname;
				std::cin >> gname;
				std::cout << "Setting new generator name: " << gname;
				fullcircle::Sequence::Ptr sq(new fullcircle::Sequence(seq->fps(), seq->width(), seq->height()));
				sq->add_sequence(seq);
				std::fstream output(srcfile.c_str(), std::ios::out | std::ios::binary);
				sq->save(output, gname, seq->generator_version());
				}
				break;
			case 5:
				{
				std::cout << "Please enter the new generator version: ";
				std::string gversion;
				std::cin >> gversion;
				std::cout << "Setting new generator version: " << gversion;
				fullcircle::Sequence::Ptr sq(new fullcircle::Sequence(seq->fps(), seq->width(), seq->height()));
				sq->add_sequence(seq);
				std::fstream output(srcfile.c_str(), std::ios::out | std::ios::binary);
				sq->save(output, seq->generator_name(), gversion);
				}
				break;
			default:
				{
				std::cout << "Unknown option. I'm out of here!" << std::endl;
				return 1;
				}
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
