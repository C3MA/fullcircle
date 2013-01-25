/**
 * This file is part of libfullcircle.
 *
 * (c) Mathias Dalheimer <md@gonium.net>, 2012
 *
 * libfullcircle is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * libfullcircle is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libfullcircle. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "common.hpp"
#include <sstream>
#include <stdint.h>

/* includes, that are needed to load the configuration */
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/positional_options.hpp>
namespace po = boost::program_options;
namespace bfs=boost::filesystem;

using namespace fullcircle;

const std::string VersionInfo::getVersion() {
  std::ostringstream oss;
  oss << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH;
  return oss.str();
}

/*
 * @param[out] pWidth  the found width in the configuration file
 * @param[out] pHeight the found height in the configuration file
 * @param[out] pFPS    the found FPS in the configuration file
 */
const void read_configuration(uint16_t *pWidth, uint16_t *pHeight, uint16_t *pFPS)
{
  uint16_t fps;
  uint16_t width;
  uint16_t height;

  po::variables_map vm; 
  bfs::path config_file;
  char* homedir = getenv("HOME");

  if (pWidth == NULL || pHeight == NULL || pFPS == NULL)
    return;

  if (homedir != NULL) {
    config_file = config_file / homedir / ".fullcirclerc";
  } else {
    config_file = config_file / "etc" / "fullcirclerc";
  }
  
 try {
    /* define the settings, we are interessted in from the configuration file */
    po::options_description generic("Generic options (config file and command line)");
    generic.add_options()
      ("width,w", po::value<std::string>(), "the width of the sequence to be generated.")
      ("height,h", po::value<std::string>(), "the height of the sequence to be generated.")
      ("fps,f", po::value<std::string>(), "the frames per second of the sequence to be generated.")
      ;
    po::notify(vm);
    /* read the configuration file */
    po::options_description config_file_options;
    config_file_options.add(generic);
    boost::filesystem::path config(config_file);
    if ( boost::filesystem::exists(config) ) {
      po::store(po::parse_config_file<char>(config_file.c_str(), config_file_options, true), vm);
    }
    po::notify(vm);

    if (vm.count("width") != 1 ) {
      std::cerr << "You must specify a width for the sequence. " << std::endl;
      width = 0;
    } else {
      std::istringstream converter(vm["width"].as<std::string>());
      if ( !( converter >> width)) {
        std::cerr << "Cannot convert width to an integer. " << std::endl;
        width = 0;
      }
    }
    *pWidth = width;

    if (vm.count("height") != 1 ) {
      std::cerr << "You must specify a height for the sequence. " << std::endl;
      height = 0;
    } else {
      std::istringstream converter(vm["height"].as<std::string>());
      if ( !( converter >> height)) {
        std::cerr << "Cannot convert height to an integer. " << std::endl;
        height = 0;
      }
    }
    *pHeight = height;

    if (vm.count("fps") != 1 ) {
      std::cerr << "You must specify the frames per second for the sequence. " << std::endl;
      fps = 0;
    } else {
      std::istringstream converter(vm["fps"].as<std::string>());
      if ( !( converter >> fps)) {
        std::cerr << "Cannot convert fps to an integer. " << std::endl;
        fps = 0;
      }
    }
    *pFPS = 0;
  } catch(...) {
    *pHeight = 0;
    *pWidth = 0;
    *pFPS = 0;
  }
}
