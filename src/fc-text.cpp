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
  fullcircle::RGB_t foreground;
  foreground.red = foreground.green = foreground.blue = 255;
	
  try {

    po::options_description generic("Generic options (config file and command line)");
		generic.add_options()
      ("width,w", po::value<std::string>(), "the width of the sequence to be generated.")
      ("height,h", po::value<std::string>(), "the height of the sequence to be generated.")
      ("fps,f", po::value<std::string>(), "the frames per second of the sequence to be generated.")
     ;
    std::ostringstream coss;
    coss << "configuration file (" << config_file << " by default).";
		po::options_description cmd("Command line options");
		cmd.add_options()
			("config", po::value<std::string>(), coss.str().c_str())
			("help,?", "produce help message")
			("version,v", "print version and exit")
      ("sequence,s", po::value<std::string>(), "the sequence file to use")
      ("text,t", po::value<std::string>(), "the text to display")
      ("moving,m", po::value<std::string>(), "time one character should be displayed before moving to the next")
      ("color,c", po::value<std::string>(), "text color in the format: '#RRGGBB' RR is red, GG green, BB blue (range of each value is 0x00-0xFF)")
			;
		std::ostringstream oss;
		oss << "Usage: " << argv[0] << " -s <FILE> -t <TEXT> ...";
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
    std::string text;
    uint32_t scrolltime;
    uint16_t fps;
    uint16_t width;
    uint16_t height;

    if (vm.count("help")) {
      std::cout << cmdline_options << std::endl;
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
      scrolltime= vm["moving"].as<uint32_t>();
      //std::string tmp = vm["moving"].as<std::string>();
      //scrolltime= atoi(tmp.c_str());
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

    if (vm.count("width") != 1 ) {
      std::cerr << "You must specify a width for the sequence. " << std::endl;
      return 1;
    } else {
      std::istringstream converter(vm["width"].as<std::string>());
      if ( !( converter >> width)) {
        std::cerr << "Cannot convert width to an integer. " << std::endl;
        return 1;
      }
    }

    if (vm.count("height") != 1 ) {
      std::cerr << "You must specify a height for the sequence. " << std::endl;
      return 1;
    } else {
      std::istringstream converter(vm["height"].as<std::string>());
      if ( !( converter >> height)) {
        std::cerr << "Cannot convert height to an integer. " << std::endl;
        return 1;
      }
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

    bfs::path sequence(sequencefile);

    try {
      fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(fps, width, height));

      fullcircle::FontRenderer::Ptr fr(new fullcircle::FontRenderer(width, height));
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
