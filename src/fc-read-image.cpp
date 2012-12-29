#include <libfullcircle/common.hpp>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/frame.hpp>
#include <libfullcircle/sprite_io.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <QImage>
#include <QPen>
namespace po = boost::program_options;
namespace bfs=boost::filesystem;

//fullcircle::Sequence::Ptr mk_demo_sequence() {
//  std::cout << "Generating demo sequence." << std::endl;

//  fullcircle::RGB_t white;
//  white.red = white.green = white.blue = 255;
//  fullcircle::RGB_t red;
//  red.red = 255; red.green = red.blue = 0;
//  fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(2,10,5));

//  for( uint32_t frameID = 0; frameID < 100; ++frameID) {
//    fullcircle::Frame::Ptr frame(new fullcircle::Frame(10,5));
//    uint16_t xpos=frameID % 10;
//    uint16_t ypos=frameID % 5;
//    std::cout << " Frame " << frameID
//      << " Xpos: " << xpos
//      << " Ypos: " << ypos << std::endl;
//    frame->set_pixel(xpos, ypos, red);
//    frame->set_pixel(0, 3, white);
//    seq->add_frame(frame);
//    frame->dump_frame(std::cout);
//  }
//  return seq;
//}


int main (int argc, char* argv[]) {

  try {
    std::ostringstream oss;
    oss << "Usage: " << argv[0] << " ACTION [additional options]";
    po::options_description desc(oss.str());
    desc.add_options()
      ("help", "produce help message")
      ("version,v", "print version and exit")
      ("sequence,s", po::value<std::string>(), "the sequence file to use")
      ("input,i", po::value<std::string>(), "the input file to use")
      ("dir,d", po::value<std::string>(), "the input directory to use")
      ("width,w", po::value<int>(), "width of animation")
      ("height,h", po::value<int>(), "height of animation")
      ("fps,f", po::value<int>(), "fps value for the animation")
      ;
    po::positional_options_description p;
    p.add("sequence", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(desc).positional(p).run(), vm);
    po::notify(vm);

    // Begin processing of commandline parameters.
    std::string sequencefile;
    std::string input = "";
    std::string indir = "";
    int width;
    int height;
    int fps;


    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 1;
    }

    if (vm.count("version")) {
      fullcircle::VersionInfo::Ptr version(new fullcircle::VersionInfo());
      std::cout << "fullcircle version " << version->getVersion() << std::endl;
      return 0;
    }

    if (! vm.count("sequence")) {
      std::cerr << "You must specify a sequence file." << std::endl;
      return 1;
    } else {
      sequencefile=vm["sequence"].as<std::string>();
    }

    if (vm.count("input")) {
      input=vm["input"].as<std::string>();
    }

    if (vm.count("dir")) {
      indir=vm["dir"].as<std::string>();
    }

		if (vm.count("input") + vm.count("dir") == 0) {
			std::cerr << "You have to specify an input file or directory." << std::endl;
			return 1;
		}

    if (! vm.count("width")) {
      std::cerr << "You must specify a frame width." << std::endl;
      return 1;
    } else {
      width=vm["width"].as<int>();
    }

    if (! vm.count("height")) {
      std::cerr << "You must specify a frame height." << std::endl;
      return 1;
    } else {
      height=vm["height"].as<int>();
    }

    if (! vm.count("fps")) {
      std::cerr << "You must specify an fps value." << std::endl;
      return 1;
    } else {
      fps=vm["fps"].as<int>();
    }

		fullcircle::SpriteIO::Ptr sprite_io(new fullcircle::SpriteIO());
		
		fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(fps, width, height));
		if ( ! input.empty() )
		{
			fullcircle::Frame::Ptr frame = sprite_io->load(input);
			for ( int i = 0; i < fps; i++)
				seq->add_frame(frame);
		} else {
			typedef std::vector<bfs::path> vec;             // store paths,
			vec v, vp;                                // so we can sort them later

			copy(bfs::directory_iterator(indir), bfs::directory_iterator(), back_inserter(vp));

			sort(vp.begin(), vp.end());             // sort, since directory iteration
			
			for(vec::iterator it = vp.begin(); it != vp.end(); it++)
			{
				fullcircle::Frame::Ptr frame = sprite_io->load(it->c_str());
				seq->add_frame(frame);
			}
		}
		std::fstream output(sequencefile.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
		seq->save(output, "fc-read-image", "0.1");
		output.close();


  } catch(std::exception& e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "Exception of unknown type!" << std::endl;
    return 1;
  }
  return 0;
}


