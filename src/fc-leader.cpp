#include <libfullcircle/common.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/frame.hpp>
#include <libfullcircle/sprite_io.hpp>
#include <libfullcircle/fontrenderer.hpp>
#include <libfullcircle/perlin_noise.hpp>
#include <libfullcircle/color_scheme_smash.hpp>
#include <libfullcircle/sfx/frame_fader.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <QApplication>

namespace po = boost::program_options;
namespace bfs=boost::filesystem;

fullcircle::Sequence::Ptr mk_leader(
    const std::string& color_string,
    uint16_t number,
    uint16_t fps,
    uint16_t width,
    uint16_t height,
		uint16_t speed
    ) 
{
  fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(fps, width, height));
  fullcircle::FrameFader::Ptr fader(new fullcircle::FrameFader(2*fps,fps));
  fullcircle::Frame::Ptr start(new fullcircle::Frame(width,height));
  start->fill_whole(fullcircle::BLACK);
  seq->add_frame(start);

 // fullcircle::RGB_t color = fullcircle::RED;
 // fullcircle::Frame::Ptr color_show(new fullcircle::Frame(width,height));
 // color_show->fill_whole(color);
 // seq = (*seq) << fader->fade(seq->get_last_frame(), color_show);

  //fullcircle::SpriteIO::Ptr sprite_io(new fullcircle::SpriteIO());
  //fullcircle::Frame::Ptr quadrat(
  //    sprite_io->load(std::string(":/sprites/quadrat_red_6x6.png")));

  //seq = (*seq) << fader->fade(seq->get_last_frame(), quadrat);

  std::ostringstream oss;
  oss << number;
	fullcircle::FontRenderer::Ptr fr(new fullcircle::FontRenderer(width, height));
  // TODO: The font has a fixed size of 5x5 pixels. Calculate baseline etc.
  // BUG: Text wird nicht gerendert
  //fr->scroll_text(seq, 2, 0, oss.str(), 500);	
  // BUG: Ab hier funktioniert das Rendering - aber Text is mittig.
  //fr->scroll_text(seq, 3, 0, oss.str(), 500);	
	//fr->write_text(seq, (width/4), (height/4), oss.str());
  fr->scroll_text(seq, width, (height-5)/2, oss.str(), speed);	


  return seq;
}



int main(int argc, char *argv[1]) {
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
      ("color,c", po::value<std::string>(), "the color string")
      ("width,w", po::value<std::string>(), "the width of the sequence to be generated.")
      ("height,h", po::value<std::string>(), "the height of the sequence to be generated.")
      ("fps,f", po::value<std::string>(), "the frames per second of the sequence to be generated.")
			("speed", po::value<std::string>(), "time in ms before scrolling text further.")
      ;

    std::ostringstream coss;
    coss << "configuration file (" << config_file << " by default).";
    po::options_description cmd("Command line options");
    cmd.add_options()
      ("config", po::value<std::string>(), coss.str().c_str())
      ("help,?", "produce help message")
      ("sequence,s", po::value<std::string>(), "the sequence file to use")
      ("version,v", "print version and exit")
      ("number,n", po::value<std::string>(), "the interaction number to show")
      ;

    std::ostringstream oss;
    oss << "Usage: " << argv[0] << "-s <FILE> -n <INT> [additional options]";
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
    std::string color;
    std::string sequencefile;
    uint32_t number;
    uint16_t fps;
    uint16_t width;
    uint16_t height;
		uint16_t speed;

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

    if (vm.count("color") != 1 ) {
      std::cerr << "You must specify the color (-c <color>). Valid colors: yellow, blue, red" << std::endl;
      return 1;
    } else {
      color=vm["color"].as<std::string>();
    }

    if (vm.count("number") != 1 ) {
      std::cerr << "You must specify the number (-n <number>)." << std::endl;
      return 1;
    } else {
      std::istringstream converter(vm["number"].as<std::string>());
      if ( !( converter >> number)) {
        std::cerr << "Cannot convert number to an integer. " << std::endl;
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

    if (vm.count("speed") != 1 ) {
      std::cerr << "You must specify a speed for the sequence. " << std::endl;
      return 1;
    } else {
      std::istringstream converter(vm["speed"].as<std::string>());
      if ( !( converter >> speed)) {
        std::cerr << "Cannot convert speed to an integer. " << std::endl;
        return 1;
      }
    }

    bfs::path sequence(sequencefile);

    //fullcircle::BadgeRenderer::Ptr badge(new fullcircle::BadgeRenderer(tmplfile));
    //std::string document=badge->render(color, number);

    fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(fps, width, height));

    seq = (*seq) << mk_leader(
        color,
        number,
        fps,
        width,
        height,
				speed
        );
    std::cout << "Saving sequence to file " << sequence << std::endl;
    std::fstream output(sequence.c_str(), 
        std::ios::out | std::ios::trunc | std::ios::binary);
    seq->save(output, "fc-leader", version->getVersion());
    output.close();



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

