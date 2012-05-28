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
  fullcircle::RGB_t white;
  white.red = white.green = white.blue = 255;
  fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(10,4,4));

  fullcircle::Frame::Ptr f0(new fullcircle::Frame(4,4));
  f0->set_pixel(0, 0, white);
  fullcircle::Frame::Ptr f1(new fullcircle::Frame(4,4));
  f1->set_pixel(1, 0, white);
  fullcircle::Frame::Ptr f2(new fullcircle::Frame(4,4));
  f2->set_pixel(2, 0, white);
  fullcircle::Frame::Ptr f3(new fullcircle::Frame(4,4));
  f3->set_pixel(3, 0, white);
  seq->add_frame(f0);
  seq->add_frame(f1);
  seq->add_frame(f2);
  seq->add_frame(f3);
  
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


