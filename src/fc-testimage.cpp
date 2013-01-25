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
namespace po = boost::program_options;
namespace bfs=boost::filesystem;

fullcircle::Sequence::Ptr mk_test_sequence(
    uint8_t fps,
    uint16_t width,
    uint16_t height
    ) {
  std::cout << "Generating test sequence." << std::endl;

  fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(fps, width, height));

  uint16_t x=0;
  uint16_t y=0;
  for( int32_t frameID = 0; frameID < width*height*2; ++frameID) {
    fullcircle::Frame::Ptr frame(new fullcircle::Frame(width,height));
    frame->fill_whole(fullcircle::BLACK);
    // generate RGB pattern
    for( uint16_t fill_y = 0; fill_y < height; fill_y += 1) {
      for( uint16_t fill_x = 0; fill_x < width; fill_x += 1) {
        uint16_t color_id=((fill_y*width)+fill_x + frameID) % 3;
        switch(color_id) {
          case 0:
            frame->set_pixel(fill_x,fill_y, fullcircle::RED);
            break;
          case 1:
            frame->set_pixel(fill_x,fill_y, fullcircle::GREEN);
            break;
          case 2:
            frame->set_pixel(fill_x,fill_y, fullcircle::BLUE);
            break;
          default:
            std::cout << "This should not occur - check RGB pattern routine." << std::endl;
            exit(-2);
        }
      }
    }
    // draw one white pixel for the current address
    if (x >= width) {
      y=y+1; x=0;
    }
    if (y >= height) {
      y=0; x=0;
    }
    frame->set_pixel(x,y, fullcircle::WHITE);
    x=x+1;
    //y=y;
    for( uint8_t repeat = 0; repeat < 6; repeat++) {
      seq->add_frame(frame);
    }
  }
  return seq;
}

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
      ;
    std::ostringstream oss;
    oss << "Usage: " << argv[0] << " -s <FILE> ...";
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
      fullcircle::ColorScheme::Ptr colors(new fullcircle::ColorSchemeSmash());

      seq = (*seq) << mk_test_sequence(
          fps,
          width,
          height
          );

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


