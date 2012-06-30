#include "badge_renderer.hpp"
#include <sstream>
#include <boost/filesystem/fstream.hpp>

using namespace fullcircle;

std::string BadgeRenderer::render() {
  bfs::ifstream ifs(_templatefile);
  std::ostringstream oss;
  std::string line;
  if (ifs.is_open()) {
    while (ifs.good()) {
      getline(ifs, line);
      oss << line << std::endl;
    }
  } else {
    std::ostringstream oss2;
    oss << "Cannot read from file " << _templatefile;
    throw RenderException(oss.str());
  }

  return oss.str();
}
