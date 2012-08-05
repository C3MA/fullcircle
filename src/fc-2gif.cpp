#include <libfullcircle/common.hpp>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/frame.hpp>
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
      ("tempfolder,t", po::value<std::string>(), "folder to write frames to")
      ("width,w", po::value<int>(), "width of animation")
      ("height,h", po::value<int>(), "height of animation")
      ;
    po::positional_options_description p;
    p.add("sequence", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(desc).positional(p).run(), vm);
    po::notify(vm);

    // Begin processing of commandline parameters.
    std::string sequencefile;
    std::string tempfolder;
    int width;
    int height;


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

    if (! vm.count("tempfolder")) {
      std::cerr << "You must specify a folder to store frame files." << std::endl;
      return 1;
    } else {
      tempfolder=vm["tempfolder"].as<std::string>();
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

    bfs::path sequence(sequencefile);
    bfs::path temp(tempfolder);

    try {
      std::cout << "Loading sequence from file " << sequence << std::endl;
      std::fstream input(sequence.c_str(),
          std::ios::in | std::ios::binary);
      fullcircle::Sequence::Ptr seq(new fullcircle::Sequence(input));
//      seq->save(output, "Testcase: check_sequence_storage", "current");
      input.close();

//      std::fstream output(gif.c_str(),
//          std::ios::out | std::ios::trunc | std::ios::binary);


      for(uint32_t framePos = 0; framePos < seq->size();framePos++) {
          fullcircle::Frame::Ptr currentFrame = seq->get_frame(framePos);
          QImage gifFrame(width,height,QImage::Format_RGB32);
          qreal pixelwidth=width/seq->width();
          qreal pixelheight=height/seq->height();
          //std::cout << "Pixel: " << pixelwidth << "x" << pixelheight << std::endl;
//          fullcircle::Frame::Ptr frame=_seq->get_frame(frameID);
          for (uint8_t x=0; x < seq->width(); ++x) {
              for (uint8_t y=0; y < seq->height(); ++y) {
                  fullcircle::RGB_t pixelcolor=currentFrame->get_pixel(x,y);

                  for(uint8_t xp = 0; xp < pixelwidth; xp++) {
                      for(uint8_t yp = 0; yp < pixelheight; yp++) {
                          gifFrame.setPixel(x*pixelwidth+xp,y*pixelheight+yp,qRgb(pixelcolor.red,
                                                                                 pixelcolor.green,
                                                                                 pixelcolor.blue));
                      }
                  }
              }
          }
          QString filename = QString("%1/%2.png").arg(temp.c_str()).arg((uint)framePos,5,10,QChar('0'));
          std::cout << "writing frame No" << framePos << " to " << filename.toStdString() << std::endl;
          gifFrame.save(filename);



      }



//      output.close();

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


