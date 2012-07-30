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
namespace po = boost::program_options;
namespace bfs=boost::filesystem;

int main (int argc, char* argv[]) {

  /*** 
   * Careful! This must be in the top level namespace of any binary, and must
   * be called only once. Just leave it here.
   */
  Q_INIT_RESOURCE(sprites);
	
  /* Set the default forgroundcolor to white */
  fullcircle::RGB_t foreground;
  foreground.red = foreground.green = foreground.blue = 255;
	
  try {
    std::ostringstream oss;
    oss << "Usage: " << argv[0] << " ACTION [additional options]";
    po::options_description desc(oss.str());
    desc.add_options()
      ("help,h", "produce help message")
      ("version,v", "print version and exit")
      ("sequence,s", po::value<std::string>(), "the sequence file to use")
      ("text,t", po::value<std::string>(), "the text to display")
      ("moving,m", po::value<std::string>(), "time one character should be displayed before moving to the next")
	  ("color,c", po::value<std::string>(), "text color in the format: '#RRGGBB' RR is red, GG green, BB blue (range of each value is 0x00-0xFF)")
     ;
    po::positional_options_description p;
    p.add("sequence", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(desc).positional(p).run(), vm);
    po::notify(vm);

    // Begin processing of commandline parameters.
    std::string sequencefile;
    std::string text;
    uint32_t scrolltime;

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
      std::cerr << "You must specify a sequence file (-s <filename>)." << std::endl;
      return 1;
    } else {
      sequencefile=vm["sequence"].as<std::string>();
    }

	  
    if (vm.count("text") != 1 ) {
      std::cerr << "You must specify an text (-t <text to display>)." << std::endl;
      return 1;
    } else {
      text=vm["text"].as<std::string>();
    }
	  
    if (vm.count("moving") != 1 ) {
		scrolltime =   500 /* time in milliseconds */;
	} else {
		std::string tmp = vm["moving"].as<std::string>();
		scrolltime= atoi(tmp.c_str());
	}
	  
	if (vm.count("color") == 1) {
		std::string tmp = vm["color"].as<std::string>();
		int red, green, blue = 0;
		try {    
			if (boost::starts_with(tmp, "#") || tmp.size() != 7)
			{				
				std::stringstream ss_red, ss_green, ss_blue;
				ss_red << std::hex << tmp.substr(1,2);
				ss_green << std::hex << tmp.substr(3,2);
				ss_blue << std::hex << tmp.substr(5,2);
				ss_red >> red;
				ss_green >> green;
				ss_blue >> blue;
				foreground.red = red;
				foreground.green = green;
				foreground.blue = blue;
			} else {
				std::cerr << "The parameter -c needs an argument like : '#02AA40'" << std::endl;
				return 1;
			}

		} catch (.../*boost::bad_cast*/) {
			std::cerr << "The parameter -c needs an argument like : '#02AA40'" << std::endl;
			return 1;
		}
	}

    bfs::path sequence(sequencefile);
	  
    try {
	  fullcircle::Sequence::Ptr seq(new fullcircle::Sequence());

	  fullcircle::FontRenderer::Ptr fr(new fullcircle::FontRenderer(seq->width(), seq->height()));
	  fr->scroll_text(seq, 0, 0, text, scrolltime /* time in milliseconds */, foreground);

      std::cout << "Saving sequence to file " << sequence << std::endl;
      std::fstream output(sequence.c_str(), 
          std::ios::out | std::ios::trunc | std::ios::binary);
      seq->save(output, "fc-perlin", version->getVersion());
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
