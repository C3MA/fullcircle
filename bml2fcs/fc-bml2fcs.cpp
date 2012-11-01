#include <libfullcircle/common.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <QApplication>
#include <QtXml/QXmlInputSource>
#include <QXmlDefaultHandler>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/frame.hpp>
#include <libfullcircle/sprite_io.hpp>
#include <libfullcircle/sfx/frame_fader.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include "SaxHandler.h"

namespace po = boost::program_options;
namespace bfs=boost::filesystem;

int main (int argc, char* argv[]) {
  // default config file path
  boost::filesystem::path config_file;
  char* homedir = getenv("HOME");
  if (homedir != NULL) {
    config_file = config_file / homedir / ".fullcirclerc";
  } else {
    config_file = config_file / "etc" / "fullcirclerc";
  }

  /***
   * Careful! This must be in the top level namespace of any binary, and must
   * be called only once. Just leave it here.
   */
  Q_INIT_RESOURCE(sprites);

  /* Set the default forgroundcolor to white */

  uint16_t fps;

  try {

    po::options_description generic("Generic options (config file and command line)");
    generic.add_options()
      //("width,w", po::value<std::string>(), "the width of the sequence to be generated.")
      //("height,h", po::value<std::string>(), "the height of the sequence to be generated.")
      ("fps,f", po::value<std::string>(), "the frames per second of the sequence to be generated.")
      ;
    std::ostringstream coss;
    coss << "configuration file (" << config_file << " by default).";
    po::options_description cmd("Command line options");
    cmd.add_options()
      ("help,h", "produce help message")
      ("version,v", "print version and exit")
      ("bml,b", po::value<std::string>(), "BML file that should be converted")
      ("sequence,s", po::value<std::string>(), "sequence file that should be generated")
      ("loud,l", "The program tells what it is doing");
    std::ostringstream oss;
    oss << "Usage: " << argv[0] << " -s <FILE> -x <HASH> ...";
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
    std::string sequencefile;
    std::string bmlfile;

    if (vm.count("help")) {
      std::cout << cmdline_options << std::endl;
      return 1;
    }

    fullcircle::VersionInfo::Ptr version(new fullcircle::VersionInfo());
    if (vm.count("version")) {
      std::cout << "fullcircle version " << version->getVersion() << std::endl;
      return 0;
    }

    bool verbose = false;
    if (vm.count("loud")) {
      verbose = true;
    }

    if (vm.count("sequence") != 1 ) {
      std::cerr << "You must specify one sequence file (-s <filename>)." << std::endl;
      return 1;
    } else {
      sequencefile=vm["sequence"].as<std::string>();
    }

    if (vm.count("bml") != 1 ) {
      std::cerr << "You must specify one bml file (-b <filename>)." << std::endl;
      return 1;
    } else {
      bmlfile=vm["bml"].as<std::string>();
    }

    if (vm.count("fps") != 1 ) {
      std::cerr << "You must specify the frames per second for the sequence. " << std::endl;
      return 1;
    } else {
      std::istringstream converter(vm["fps"].as<std::string>());
      if ( !( converter >> fps)) {
        std::cerr << "Cannot convert fps to an integer. " << std::endl;
        return 1;
      }
    }

    try {
		fullcircle::Sequence::Ptr sum;

		// init the SaxHandler for Xml  parsing
		SaxHandler handler(fps, verbose);

		QXmlSimpleReader reader;
		reader.setContentHandler(&handler);
                reader.setErrorHandler(&handler);

		// Open the BML file for parsing
		QFile file(bmlfile.c_str());
		if (!file.open(QFile::ReadOnly | QFile::Text)) {
			std::cout << "ould not open file" << std::endl;
			return -1;
		}

        QXmlInputSource inputSource(&file);

        // parse the bml. do it
        reader.parse(inputSource);

        file.close();

        // save the fcs
		std::fstream output(sequencefile.c_str(),
							std::ios::out | std::ios::trunc | std::ios::binary);

		handler.GetSequence()->save(output, "fc-bml2fcs", version->getVersion());
		output.close();

    } catch (fullcircle::GenericException& ex) {
      std::cout << "Caught exception: " << ex.what() << std::endl;
      exit(1);
    }
  } catch(std::exception& e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "Exception of unknown type!" << std::endl;
    return 1;
  }
  return 0;
}
