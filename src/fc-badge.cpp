#include <libfullcircle/common.hpp>
#include <libfullcircle/printer/printerinterface.hpp>
#include <libfullcircle/printer/badge_renderer.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
#include <iostream>
#include <sstream>
namespace po = boost::program_options;
namespace bfs=boost::filesystem;


int main(int argc, char *argv[1]) {
  try {
    std::ostringstream oss;
    oss << "Usage: " << argv[0] << " ACTION [additional options]";
    po::options_description desc(oss.str());
    desc.add_options()
      ("help", "produce help message")
      ("version,v", "print version and exit")
      ("simulate,s", "do not print the badge - just dump it on the console.")
      ("device,d", po::value<std::string>(), "the serial device of the printer")
      ("template,t", po::value<std::string>(), "the template file to use")
     ;
    po::positional_options_description p;
    p.add("device", 1);
    p.add("template", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(desc).positional(p).run(), vm);
    po::notify(vm);

    // Begin processing of commandline parameters.
    std::string device;
    std::string tmpl;

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 1;
    }

    fullcircle::VersionInfo::Ptr version(new fullcircle::VersionInfo());
    if (vm.count("version")) {
      std::cout << "fullcircle version " << version->getVersion() << std::endl;
      return 0;
    }

    if (vm.count("device") != 1 ) {
      std::cerr << "You must specify the printer's device (-d <path>)." << std::endl;
      return 1;
    } else {
      device=vm["device"].as<std::string>();
    }

    if (vm.count("template") != 1 ) {
      std::cerr << "You must specify the file containing the badge template (-t <path>)." << std::endl;
      return 1;
    } else {
      tmpl=vm["template"].as<std::string>();
    }

    bfs::path devicefile(device);
    bfs::path tmplfile(tmpl);

    fullcircle::BadgeRenderer::Ptr badge(new fullcircle::BadgeRenderer(tmplfile));
    std::string document=badge->render();

    if (vm.count("simulate") == 0) {
      fullcircle::PrinterInterface::Ptr printer(
          new fullcircle::PrinterInterface(devicefile));
      printer->print(document);
      printer->cut_paper();
    } else {
      std::cout << document << std::endl;
    }

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

