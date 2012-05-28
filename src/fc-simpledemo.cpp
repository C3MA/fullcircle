#include <libfullcircle/common.hpp>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/frame.hpp>
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
    frame->set_pixel(0, 0, white);
    seq->add_frame(frame);
    frame->dump_frame(std::cout);
  }
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
      ;
    po::positional_options_description p;
    p.add("sequence", 1);

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

    if (! vm.count("sequence")) {
      std::cerr << "You must specify a sequence file." << std::endl;
      return 1;
    } else {
      sequencefile=vm["sequence"].as<std::string>();
    }

    bfs::path sequence(sequencefile);

    try {
      fullcircle::Sequence::Ptr seq(mk_demo_sequence());
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


