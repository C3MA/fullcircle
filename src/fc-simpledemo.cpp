#include <libfullcircle/common.hpp>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/frame.hpp>
#include <libfullcircle/fontrenderer.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
namespace po = boost::program_options;
namespace bfs=boost::filesystem;

fullcircle::Sequence::Ptr mk_demo_sequence() {
  std::cout << "Generating demo sequence." << std::endl;

  fullcircle::RGB_t white;
  white.red = white.green = white.blue = 255;
  fullcircle::RGB_t red;
  red.red = 255; red.green = red.blue = 0;
  fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(2,10,5));

  for( uint32_t frameID = 0; frameID < 100; ++frameID) {
    fullcircle::Frame::Ptr frame(new fullcircle::Frame(10,5));
    uint16_t xpos=frameID % 10;
    uint16_t ypos=frameID % 5;
    std::cout << " Frame " << frameID 
      << " Xpos: " << xpos 
      << " Ypos: " << ypos << std::endl;
    frame->set_pixel(xpos, ypos, red);
    frame->set_pixel(0, 3, white);
    seq->add_frame(frame);
    frame->dump_frame(std::cout);
  }
  return seq;
}

fullcircle::Sequence::Ptr mk_demo_fullscreen() {
	std::cout << "Generating fullscreen demo sequence." << std::endl;
	
	fullcircle::RGB_t white;
	white.red = white.green = white.blue = 255;
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(25,10,5));
	
	int endcounter = 0;
	
	for( uint32_t frameID = 0; frameID < 1000; ++frameID) {
		fullcircle::Frame::Ptr frame(new fullcircle::Frame(10,5));
		frame->fill_whole(white);
		
		if (white.blue > 0 && white.blue <= 255) {
			white.blue -= 2;
		} else if (white.green > 0 && white.green <= 255) {
			white.green -= 2;
		} else if (white.red > 0 && white.red <= 255) {
			white.red -= 2;
		} else {
			endcounter++;
			switch (endcounter) {
				case 1:
					white.blue = 255;
					break;
				case 2:
					white.green = 255;
					break;
				case 3:
					white.red = 255;
					break;
				case 4:
					white.blue = 255;
					white.green = 255;
					white.red = 255;
					endcounter = 0;
					break;
				default:
					break;
			}
		}
		
		if (white.blue > 256) {
			white.blue = 0;
		}
		if (white.green > 256) {
			white.green = 0;
		}
		if (white.red > 256) {
			white.red = 0;
		}
		
		
		seq->add_frame(frame);
		
		frame->dump_frame(std::cout);
	}
	return seq;
}

fullcircle::Sequence::Ptr mk_demo_fade() {
	std::cout << "Generating gradient demo sequence." << std::endl;
	
	fullcircle::RGB_t from;
	from.red = from.green = 0; from.blue = 255;
	fullcircle::RGB_t to;
	to.red = to.blue = 0; to.green = 255; 
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(25,10,5));
	
	for( uint32_t frameID = 0; frameID < 50; ++frameID) {
		fullcircle::Frame::Ptr frame(new fullcircle::Frame(10,5));
		frame->fill_gradient_vertical(from, to);		
		seq->add_frame(frame);		
		frame->dump_frame(std::cout);
	}
	
	for( uint32_t frameID = 0; frameID < 50; ++frameID) {
		fullcircle::Frame::Ptr frame(new fullcircle::Frame(10,5));
		frame->fill_gradient_horizontal(from, to);		
		seq->add_frame(frame);		
		frame->dump_frame(std::cout);
	}
	
	/***** from blue to blue ... *******/
	from.blue = 255; from.red = from.green = 0;
	to.blue = 50; to.red = to.green = 0;
	for( uint32_t frameID = 0; frameID < 50; ++frameID) {
		fullcircle::Frame::Ptr frame(new fullcircle::Frame(10,5));
		frame->fill_gradient_horizontal(from, to);		
		seq->add_frame(frame);		
		frame->dump_frame(std::cout);
	}
	
	
	return seq;
}

fullcircle::Sequence::Ptr mk_demo_text() {
	std::cout << "Generating text demo sequence." << std::endl;
	fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(25,10,5));
	
	fullcircle::RGB_t color;
	color.red = color.green = color.blue = 0; 
	fullcircle::Frame::Ptr frame(new fullcircle::Frame(10,5));
	frame->fill_whole(color); // is white at the moment
	seq->add_frame(frame);
	color.blue = 255; // now its blue ;-)
	
	fullcircle::FontRenderer::Ptr fr(new fullcircle::FontRenderer(10, 5));
	fr->load_font("font1.xbm"); // This file describes how to display each character
	fr->write_text(seq, 0, 0, "C3",color);
	
	return seq;
}

int main (int argc, char* argv[]) {

  try {
    std::ostringstream oss;
    oss << "Usage: " << argv[0] << " ACTION [additional options]";
    po::options_description desc(oss.str());
    desc.add_options()
      ("help,h", "produce help message")
      ("version,v", "print version and exit")
      ("sequence,s", po::value<std::string>(), "the sequence file to use")
	  ("fullscreen,f", po::value<std::string>(), "the sequence file to use [for a fullscreen animation]")
  	  ("gradient,g", po::value<std::string>(), "the sequence file to use [for a gradient animation]")
	  ("text,t", po::value<std::string>(), "the sequence file to use [for a text animation]")
      ;
    po::positional_options_description p;
//    p.add("sequence", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(desc).positional(p).run(), vm);
    po::notify(vm);

    // Begin processing of commandline parameters.
    std::string sequencefile;

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 1;
    }

    if (vm.count("version")) {
      fullcircle::VersionInfo::Ptr version(new fullcircle::VersionInfo());
      std::cout << "fullcircle version " << version->getVersion() << std::endl;
      return 0;
    }

	int type = 0;
    if (vm.count("sequence") + vm.count("fullscreen") + vm.count("gradient") + vm.count("text") <= 0) {
      std::cerr << "You must specify a sequence file." << std::endl;
      return 1;
    } else {
		if (vm.count("sequence") > 0) {
			type = 1;
			sequencefile=vm["sequence"].as<std::string>();
		} else if (vm.count("fullscreen") > 0) {
			sequencefile=vm["fullscreen"].as<std::string>();
			type = 2;
		} else if (vm.count("gradient") > 0) {
			sequencefile=vm["gradient"].as<std::string>();
			type = 3;
		} else if (vm.count("text") > 0) {
			sequencefile=vm["text"].as<std::string>();
			type = 4;
		}
		
    }
	  
    bfs::path sequence(sequencefile);

    try {
		fullcircle::Sequence::Ptr seq;
		switch (type) {
			case 1:
				seq = mk_demo_sequence();
				break;
			case 2:
				seq = mk_demo_fullscreen();
				break;
			case 3:
				seq = mk_demo_fade();
				break;
			case 4:
				seq = mk_demo_text();
				break;
			default:
				break;
		}
      std::cout << "Saving sequence to file " << sequence << std::endl;
      std::fstream output(sequence.c_str(), 
          std::ios::out | std::ios::trunc | std::ios::binary);
      seq->save(output, "Testcase: check_sequence_storage", "current");
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


