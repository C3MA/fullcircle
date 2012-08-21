#include <libfullcircle/common.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <QApplication>
#include <boost/filesystem.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/frame.hpp>
#include <libfullcircle/sprite_io.hpp>
#include <libfullcircle/fontrenderer.hpp>
#include <libfullcircle/perlin_noise.hpp>
#include <libfullcircle/color_scheme_smash.hpp>
#include <libfullcircle/sfx/frame_fader.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <libfullcircle/flowmap.hpp>

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
      ("hash,x", po::value<std::string>(), "the seed hash for the visualization")
      ("increase,i", "increase the speed of the colored 'water'")
      ("maximum,m", po::value<int>(), "the maximum frames to render")
			;
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
    std::string hash;
    uint16_t fps;
    uint16_t width;
    uint16_t height;
    bool speedFlow = false;
    int maximumFrames = 0;

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

    if (vm.count("hash") != 1 ) {
      std::cerr << "You must specify an hash (-x <hash>)." << std::endl;
      return 1;
    } else {
      hash=vm["hash"].as<std::string>();
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

    if (vm.count("increase") == 1 ) {
	speedFlow = true;
    }

    if (vm.count("maximum") == 1 ) {
	maximumFrames = vm["maximum"].as<int>();
    }

    bfs::path sequence(sequencefile);

	  
    // Construct seed
    uint64_t seed=0;
    for( uint16_t i = 0; i < hash.length(); ++i) {
      seed += hash[i];
    }
    std::cout << "Constructed seed " << (uint16_t) seed << " from hash." << std::endl;

    try {
      fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(fps, width, height));
      fullcircle::ColorScheme::Ptr colors(new fullcircle::ColorSchemeSmash());
		int frameCount = 0;
	  /* specify the first frame, where the color should flow down */
	  fullcircle::Frame::Ptr startFrame(new fullcircle::Frame(width, height));
	  startFrame->fill_whole(colors->get_background());
		
		// build a better algorithom to spread color on the first frame
		fullcircle::RGB_t white;
		white.red = white.green = white.blue = 255;
		
		startFrame->fill_whole(white); //colors->get_secondary(),  get_primary()
		
		fullcircle::FlowMap::Ptr fm(new fullcircle::FlowMap());
		fm->init(hash, width, height);
		fm->start_points(startFrame);
		
		std::cerr << "The hills:" << std::endl;  //Hills		
		fm->dump_hills(std::cerr);
//		std::cerr << "The first frame was initialized" << std::endl;  //Debug

		
		
		while (fm->has_changed() && (maximumFrames == 0 || frameCount < maximumFrames) ) {
			// make a deep copy of the frame
			fullcircle::Frame::Ptr actFrame(new fullcircle::Frame(width, height));
			actFrame->set_pixel(0,0, fm->get_next());
			seq->add_frame( actFrame );
//			actFrame->dump_frame(std::cerr); // Debug
//			std::cerr << "---------------------" << std::endl;  //Debug 2		
			frameCount++;
		}

	if (maximumFrames > 0)
	{
		std::cout << "Generated " << frameCount << " frames, the limit was " << maximumFrames << std::endl;
	}
	else
	{
		std::cout << "Generated " << frameCount << " frames." << std::endl;
	}		
      std::cout << "Saving sequence to file " << sequence << std::endl;
      std::fstream output(sequence.c_str(), 
          std::ios::out | std::ios::trunc | std::ios::binary);
      seq->save(output, "fc-flowmap", version->getVersion());
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


