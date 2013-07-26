#include <libfullcircle/common.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <QApplication>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/frame.hpp>
#include <libfullcircle/sprite_io.hpp>
#include <libfullcircle/fontrenderer.hpp>
#include <libfullcircle/perlin_noise.hpp>
#include <libfullcircle/color_scheme_smash.hpp>
#include <libfullcircle/sfx/frame_fader.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace po = boost::program_options;
namespace bfs=boost::filesystem;

int main (int argc, char* argv[]) {

  /*** 
   * Careful! This must be in the top level namespace of any binary, and must
   * be called only once. Just leave it here.
   */
  Q_INIT_RESOURCE(sprites);
	
	
  try {
    std::ostringstream oss;
    oss << "Usage: " << argv[0] << " ACTION [additional options]";
    po::options_description desc(oss.str());
    desc.add_options()
      ("help,h", "produce help message")
      ("version,v", "print version and exit")
      ("sequence,s", po::value<std::string>(), "sequence file to read")
      ("prefix,p", po::value<std::string>(), "sequence file prefix for all snippets")
      ("length,l", po::value<std::string>(), "length in milliseconds of each snippet")
	;
    po::variables_map vm;
    po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    po::store(parsed, vm);
    po::notify(vm);

    uint32_t length;
	
    // Begin processing of commandline parameters.
    std::string sequencefile;
    // store the unkown attributes (aka input files)
    std::vector<std::string> input = collect_unrecognized(parsed.options, po::include_positional);
	  
    std::string prefixfile;

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

    if (vm.count("prefix") != 1 ) {
      std::cerr << "You must specify the prefix of the snippet sequence files (-p <filename>)." << std::endl;
      return 1;
    } else {
      prefixfile=vm["prefix"].as<std::string>();
    }

    if (vm.count("length") != 1 ) {
      std::cerr << "Length for each snippet must be defined in milliseconds. (-l <duration in ms>)" << std::endl;
      return 1;
    } else {
      std::istringstream converter(vm["length"].as<std::string>());
      if ( !( converter >> length)) {
        std::cerr << "Cannot convert length to an integer. " << std::endl;
        return 1;
      }
    }
  

    try {
		std::cout << "Loading sequence  from file " << sequencefile << std::endl;
		std::fstream inputStream(sequencefile.c_str(), std::ios::in | std::ios::binary);
		fullcircle::Sequence::Ptr inputSeq(new fullcircle::Sequence(inputStream));
		inputStream.close();
		
		uint32_t frameLength = inputSeq->fps() * length / 1000; /* amount of frames for one snippet */
		
		for(uint32_t frameCount=0; frameCount < inputSeq->size(); frameCount += frameLength)
		{
			boost::format fmt("%03d");
			fmt % ((frameCount / frameLength) + 1);
			std::string snippetfile (prefixfile + fmt.str() + ".seq");
			std::cout << "" << snippetfile << " with " << frameCount << " - " << (frameCount+frameLength) << std::endl;
			
			fullcircle::Sequence::Ptr snippetSeq(new fullcircle::Sequence( (uint16_t) inputSeq->fps(),
								inputSeq->width(), inputSeq->height() ));
			inputSeq->extract_sequence(snippetSeq, frameCount, frameLength);
			std::fstream output(snippetfile.c_str(), 
					std::ios::out | std::ios::trunc | std::ios::binary);
			snippetSeq->save(output, "fc-split", version->getVersion());// why the hell is this also stored?
			output.close();
		}

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
