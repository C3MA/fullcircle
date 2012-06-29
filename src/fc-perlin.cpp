#include <libfullcircle/common.hpp>
#include <iostream>
#include <fstream>
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

fullcircle::Sequence::Ptr mk_perlin_noise(
    fullcircle::ColorScheme::Ptr colors, 
    uint16_t seed,
    uint8_t fps,
    uint8_t width,
    uint8_t height
    ) {
  std::cout << "Generating Perlin noise demo sequence." << std::endl;

  fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(fps, width, height));
  fullcircle::FrameFader::Ptr fader(new fullcircle::FrameFader(5,fps));

  fullcircle::PerlinNoise::Ptr noize(new fullcircle::PerlinNoise(
        3,    // octaves
        .42, // freq
        2.4,   // amplitude
        seed    // seed
        ));

  for( uint32_t frameID = 0; frameID < 100; ++frameID) {
    fullcircle::Frame::Ptr frame(new fullcircle::Frame(width,height));
    frame->fill_whole(colors->get_background());
    for( uint16_t x = 0; x < width; ++x) {
      for( uint16_t y = 0; y < height; ++y) {
        float n=noize->get_3d(x/8.0f, y/8.0f, frameID/10.0f);
        // determine color
        if (0<=n && n<0.3) {
          frame->set_pixel(x,y,colors->get_primary());
        } else if (0.3<=n && n<0.7) {
          frame->set_pixel(x,y,colors->get_secondary());
        } else if (0.7<=n && n<=1.0) {
          frame->set_pixel(x,y,colors->get_background());
        } else {
          std::cout << "Error: Unknown noise value: " << n << std::endl;
        }
      }
    }
    if (seq->size() == 0)
      seq->add_frame(frame);
    else
      seq = (*seq) << fader->fade(seq->get_last_frame(), frame);
  }
  return seq;
}

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
      ("help", "produce help message")
      ("version,v", "print version and exit")
      ("sequence,s", po::value<std::string>(), "the sequence file to use")
      ("hash,h", po::value<std::string>(), "the seed hash for the visualization")
     ;
    po::positional_options_description p;
    p.add("sequence", 1);
    p.add("hash", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(desc).positional(p).run(), vm);
    po::notify(vm);

    // Begin processing of commandline parameters.
    std::string sequencefile;
    std::string hash;

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

    if (vm.count("hash") != 1 ) {
      std::cerr << "You must specify an hash (-h <hash>)." << std::endl;
      return 1;
    } else {
      hash=vm["hash"].as<std::string>();
    }

    bfs::path sequence(sequencefile);

    uint8_t fps=12;
    uint8_t width=16;
    uint8_t height=9;
    // Construct seed
    uint64_t seed=0;
    for( uint16_t i = 0; i < hash.length(); ++i) {
      seed += hash[i];
    }
    std::cout << "Constructed seed " << (uint16_t) seed << " from hash." << std::endl;

    try {
      fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(12,16,9));
      fullcircle::ColorScheme::Ptr colors(new fullcircle::ColorSchemeSmash());

      seq = (*seq) << mk_perlin_noise(
          colors,
          (uint16_t) seed, 
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


