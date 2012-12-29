#include <libfullcircle/common.hpp>
#include <libfullcircle/sequence.hpp>
#include <libfullcircle/net/client.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <fstream>

#include "snake.h"

#define FIFO_FILE "/tmp/snake_cmd"

namespace po = boost::program_options;
namespace bfs=boost::filesystem;

/* The SnakeClient is a reference implementation of a client for fullcircle.
 */
namespace fullcircle {
  class SnakeClient : public Client {
    public:
      SnakeClient(boost::asio::io_service *client_io_service, boost::asio::ip::tcp::resolver::iterator *iterator, uint16_t gridX, uint16_t gridY)
        : Client(client_io_service, iterator), _gridX(gridX), _gridY(gridY)
    {
	    _fp = open(FIFO_FILE, O_RDONLY | O_NONBLOCK);
      _snake = NULL;
      _framesAfterDeath=5;
    }

      ~SnakeClient()
      {
              if(_snake)
                      delete _snake;
              close(_fp);
      }

      void idle()
      {
        _snake = new Snake(_gridX,_gridY);
        // read fcs file in order to get the meta information
//        std::cout << "Reading " << _file.c_str() << std::endl;
//        std::fstream input(_file.c_str(), std::ios::in | std::ios::binary);
//        _sequence.reset(new fullcircle::Sequence(input));
//        input.close();

        // fill meta information with content of the fcs-file
        fullcircle::BinarySequenceMetadata* meta = new fullcircle::BinarySequenceMetadata();
        meta->set_frames_per_second(1);
        meta->set_width(_gridX);
        meta->set_height(_gridY);
        meta->set_generator_name("snake");
        meta->set_generator_version("0.1");

        send_request("green", 14, meta);
      }

      virtual void ack()
      {
        std::cout << "Ack" << std::endl;
      }

      virtual void nack()
      {
      }

      virtual void start()
      {
        _start = false;
        _ifs = 1000/1;
//        _frameId = 0;
        _timer.expires_from_now(boost::posix_time::milliseconds(_ifs));
        _timer.async_wait(boost::bind(&fullcircle::SnakeClient::send_frame, this));
      }

      void send_frame()
      {
        if (_snake->alive()/* _frameId < _sequence->size()*/ )
        {
                for(int k=0;k<10;k++){
    char readbuf[5];
		int count = read(_fp,readbuf,1);
		if(count > 0)
		{
			readbuf[count]=0;
      _start = true;
	//		printf("Received string: %s\n", readbuf);
			switch(readbuf[0])
			{
				case 'r':
				case 'R':
				_snake->dir = 1;
				break;
				case 'd':
				case 'D':
				_snake->dir = 2;
				break;
				case 'l':
				case 'L':
				_snake->dir = 3;
				break;
				case 'u':
				case 'U':
				_snake->dir = 4;
				break;
				//default:
				//continue;
			}
		}
    }
	    if(_start)
	    {
  		  _snake->move();
	    }
	    _ifs = 1000 / (1.0 + _snake->length*0.05);/// (((_snake->length - 2) / 15)+1) ;
	  _snake->render();

          fullcircle::Frame::Ptr frame = Frame::Ptr(new fullcircle::Frame(_gridX, _gridY));
	  for(int i=0;i<_gridX;i++)
	  {
		  for( int j=0;j<_gridY;j++)
		  {
              if(_snake->grid[i*_gridY+j]==0x11)
                frame->set_pixel(i,j,255,0,0);
              else if(_snake->grid[i*_gridY+j]==0xff)
                frame->set_pixel(i,j,0,0,255);
              else if(_snake->grid[i*_gridY+j]==0x00)
                frame->set_pixel(i,j,128,128,128);
              else
                frame->set_pixel(i,j,0,_snake->grid[i*_gridY+j],0);
		  }
	  }
          _lastframe = frame;
          _dispatcher->send_frame(frame);

          _timer.expires_from_now(boost::posix_time::milliseconds(_ifs));
          _timer.async_wait(boost::bind(&fullcircle::SnakeClient::send_frame, this));
	} else if(_framesAfterDeath > 0)
	{
          fullcircle::Frame::Ptr frame = Frame::Ptr(new fullcircle::Frame(_gridX, _gridY));
	  for(int i=0;i<_gridX;i++)
	  {
		  for( int j=0;j<_gridY;j++)
		  {
              if(_snake->grid[i*_gridY+j]==0x11)
                frame->set_pixel(i,j,255,0,0);
              else if(_snake->grid[i*_gridY+j]==0xff)
                frame->set_pixel(i,j,0,0,255);
              else if(_snake->grid[i*_gridY+j]==0x00)
                frame->set_pixel(i,j,255*(_framesAfterDeath%2),0,0);
              else
                frame->set_pixel(i,j,0,_snake->grid[i*_gridY+j],0);
		  }
	  }
          _lastframe = frame;
          _dispatcher->send_frame(frame);

          _timer.expires_from_now(boost::posix_time::milliseconds(_ifs));
          _timer.async_wait(boost::bind(&fullcircle::SnakeClient::send_frame, this));
	_framesAfterDeath--;
        } else {
          _dispatcher->send_eos();
          _state = IDLE;
          exit(0);
        }
      }

    private:
//      std::string _file;
//      fullcircle::Sequence::Ptr _sequence;
      uint16_t _gridX;
      uint16_t _gridY;
      Snake *_snake;
      uint32_t _frameId;
      double _ifs;

      int _fp;
      fullcircle::Frame::Ptr _lastframe;
      bool _start;
      int _framesAfterDeath;
  };
}

//void foobar(int signum)
//{
//        std::cout << "FOOBAR!" << std::endl;
//        return;
//}

int main (int argc, char const* argv[]) {
//  signal(SIGSEGV,foobar);
  std::cout << "FullCircle Snake Game" << std::endl;
  // default config file path
  boost::filesystem::path config_file;
  char* homedir = getenv("HOME");
  if (homedir != NULL) {
    config_file = config_file / homedir / ".fullcirclerc";
  } else {
    config_file = config_file / "etc" / "fullcirclerc";
  }


  try {
    po::options_description generic("Generic options (config file and command line)");
    generic.add_options()
      ("port,p", po::value<std::string>(), "the width of the sequence to be generated.")
      ("server,s", po::value<std::string>(), "the height of the sequence to be generated.")
      ;
    std::ostringstream coss;
    coss << "configuration file (" << config_file << " by default).";
    po::options_description cmd("Command line options");
    cmd.add_options()
      ("config", po::value<std::string>(), coss.str().c_str())
      ("input,i", po::value<std::string>(), "input sequence")
      ("help,?", "produce help message")
      ("version,v", "print version and exit")
      ("width,w", po::value<std::string>(), "the width of the sequence to be generated")
      ("height,h", po::value<std::string>(), "the height of the sequence to be generated")
      ;
    std::ostringstream oss;
    oss << "Usage: " << argv[0] << " -s <IP> -p <PORT> ...";
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
    std::string server;
//    std::string srcfile;
    uint16_t port;
    uint16_t height,width;

    if (vm.count("help")) {
      std::cout << cmdline_options << std::endl;
      return 1;
    }

    fullcircle::VersionInfo::Ptr version(new fullcircle::VersionInfo());
    if (vm.count("version")) {
      std::cout << "fullcircle version " << version->getVersion() << std::endl;
      return 0;
    }

    if (vm.count("server") != 1 ) {
      std::cerr << "You must specify a server (-s <HOST/IP>)." << std::endl;
      return 1;
    } else {
      server=vm["server"].as<std::string>();
    }

    if (vm.count("port") != 1 ) {
      std::cerr << "You must specify a port. " << std::endl;
      return 1;
    } else {
      std::istringstream converter(vm["port"].as<std::string>());
      if ( !( converter >> port)) {
        std::cerr << "Cannot convert port to an integer. " << std::endl;
        return 1;
      }
    }

    if (vm.count("height") != 1 ) {
      std::cerr << "You must specify a height (-h <HEIGHT>)." << std::endl;
      return 1;
    } else {
      std::istringstream converter(vm["height"].as<std::string>());
      if ( !( converter >> height)) {
        std::cerr << "Cannot convert height to an integer. " << std::endl;
        return 1;
    }
    }
    if (vm.count("width") != 1 ) {
      std::cerr << "You must specify a width (-w <WIDTH>)." << std::endl;
      return 1;
    } else {
      std::istringstream converter(vm["width"].as<std::string>());
      if ( !( converter >> width)) {
        std::cerr << "Cannot convert width to an integer. " << std::endl;
        return 1;
      }
    }

//    if (vm.count("input") != 1 ) {
//      std::cerr << "You must specify an input file." << std::endl;
//      return 1;
//    } else {
//      srcfile = vm["input"].as<std::string>();
//    }

    boost::asio::io_service client_io_service;
    boost::asio::ip::tcp::resolver resolver(client_io_service);
    std::ostringstream oss2;
    oss2 << port;
    boost::asio::ip::tcp::resolver::query query(
        server, oss2.str());
    boost::asio::ip::tcp::resolver::iterator iterator = 
      resolver.resolve(query);

    umask(0);
	  mknod(FIFO_FILE, S_IFIFO|0666, 0);


    fullcircle::SnakeClient client(&client_io_service, &iterator, width, height);
    client.run();

    // enter the asio event loop
    client_io_service.run();

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

