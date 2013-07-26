#include <QApplication>
#include <libfullcircle/common.hpp>
#include <libfullcircle/simulator_main_window.hpp>
#include <iostream>
#include <boost/filesystem.hpp>
#include <libfullcircle/sequence.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/asio.hpp>
#include <libfullcircle/net/net_server.hpp>

namespace po = boost::program_options;
namespace bfs=boost::filesystem;

int main (int argc, char* argv[]) {

  try {
    std::ostringstream oss;
    oss << "Usage: " << argv[0] << " ACTION [additional options]";
    po::options_description desc(oss.str());
    desc.add_options()
      ("help,h", "produce help message")
      ("version,v", "print version and exit")
      ("sequence,s", po::value<std::string>(), "the sequence file to use")
      ("port,p", po::value<int>(), "the port to listen at")
      ;
    po::positional_options_description p;
    p.add("sequence", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(desc).positional(p).run(), vm);
    po::notify(vm);

    // Begin processing of commandline parameters.
    std::string sequencefile;
    int port;

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 1;
    }

    if (vm.count("version")) {
      fullcircle::VersionInfo::Ptr version(new fullcircle::VersionInfo());
      std::cout << "fullcircle version " << version->getVersion() << std::endl;
      return 0;
    }

    if (vm.count("sequence") + vm.count("port") < 1) {
      std::cerr << "You must specify a sequence file." << std::endl;
      return 1;
    }

    QApplication app(argc, argv);
    Q_INIT_RESOURCE(sprites);

    try {
      if (vm.count("sequence")) {
        sequencefile=vm["sequence"].as<std::string>();

        bfs::path sequence(sequencefile);

        //app.setStyle("plastique");
        fullcircle::SimulatorMainWindow mainWindow(sequence);
        mainWindow.show();

        return app.exec();
      }

      if (vm.count("port")) {
        port = vm["port"].as<int>();

        boost::asio::io_service server_io_service;
        boost::asio::ip::tcp::endpoint server_endpoint(
            boost::asio::ip::tcp::v4(), port);
        fullcircle::NetServer::Ptr server = fullcircle::NetServer::create(
            server_io_service, server_endpoint);

        fullcircle::SimulatorMainWindow mainWindow(server);
        mainWindow.show();
        //server->join();

        return app.exec();
      }
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


