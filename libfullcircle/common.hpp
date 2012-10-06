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

#ifndef LIBKLIO_COMMON_HPP
#define LIBKLIO_COMMON_HPP 1


/* Include TR1 shared ptrs in a portable way. 
#include <cstddef> // for __GLIBCXX__
#ifdef __GLIBCXX__
#  include <tr1/memory>
#else
#  ifdef __IBMCPP__
#    define __IBMCPP_TR1__
#  endif
#  include <memory>
#endif
*/

#include <boost/shared_ptr.hpp>

//#define ENABLE_LOGGING 0
#include <libfullcircle/config.h>

#ifdef ENABLE_LOGGING
#include <iostream>
#define LOG(msg) std::cout << msg << std::endl;
#else
#define LOG(msg) 
#endif

#include <stdint.h>
#include <string>

#include <libfullcircle/error.hpp>
#include <libfullcircle/color_scheme.hpp>

namespace fullcircle {
  class VersionInfo {
    public:
      typedef boost::shared_ptr<VersionInfo> Ptr;
      VersionInfo () {};
      virtual ~VersionInfo() {};
      const std::string getVersion();

    private:
      VersionInfo (const VersionInfo& original);
      VersionInfo& operator= (const VersionInfo& rhs);
      
  };

};

#endif /* LIBKLIO_COMMON_HPP */
