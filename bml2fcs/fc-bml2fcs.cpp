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

  /*** 
   * Careful! This must be in the top level namespace of any binary, and must
   * be called only once. Just leave it here.
   */
  Q_INIT_RESOURCE(sprites);
	
  /* Set the default forgroundcolor to white */
	
  try {
    std::ostringstream oss;
    oss << "Usage: " << argv[0] << " ACTION [additional options] <sequence1> <sequence2> ...";
    po::options_description desc(oss.str());
    desc.add_options()
      ("help,h", "produce help message")
      ("version,v", "print version and exit")
      ("bml,b", po::value<std::string>(), "BML file that should be converted")
      ("sequence,s", po::value<std::string>(), "sequence file that should be generated");
    po::variables_map vm;
    po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    po::store(parsed, vm);
    po::notify(vm);

	
    // Begin processing of commandline parameters.
    std::string sequencefile;
	// store the unkown attributes (aka input files)
	std::vector<std::string> input = collect_unrecognized(parsed.options, po::include_positional);
    std::string bmlfile;
	  
    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 1;
    }

    fullcircle::VersionInfo::Ptr version(new fullcircle::VersionInfo());
    if (vm.count("version")) {
      std::cout << "fullcircle version " << version->getVersion() << std::endl;
      return 0;
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
    //bfs::path sequence(sequencefile);
	  
    try {
		fullcircle::Sequence::Ptr sum;
		
		// Open BML
		SaxHandler handler;

		QXmlSimpleReader reader;
		reader.setContentHandler(&handler);
                reader.setErrorHandler(&handler);
		QFile file(bmlfile.c_str());
		if (!file.open(QFile::ReadOnly | QFile::Text)) {
			std::cout << "ould not open file" << std::endl;
			return -1;
		}
                
                QXmlInputSource inputSource(&file);

                reader.parse(inputSource);
/*
		// iterate over all input files and add them.
		for(unsigned int i=0; i < input.size(); i++)
		{
			bfs::path sequence(input[i]);
		
			std::cout << "Loading sequence" << (i + 1) << " from file " << sequence << std::endl;
			std::fstream inputStream(sequence.c_str(), std::ios::in | std::ios::binary);
			fullcircle::Sequence::Ptr inputSeq(new fullcircle::Sequence(inputStream));
			inputStream.close();
			
			// add one sequence to the next.
			if (sum == NULL)
				sum = inputSeq;
			else 
				sum = sum->add(0, inputSeq, ringBufferFunctionality);
		}
*/	
/*	
		std::fstream output(sequence.c_str(), 
							std::ios::out | std::ios::trunc | std::ios::binary);
		sum->save(output, "fc-add", version->getVersion());// why the hell is this also stored?
		output.close();
*/		
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
