#ifndef LIBFULLCIRCLE_PRINTER_BADGE_RENDERER_HPP
#define LIBFULLCIRCLE_PRINTER_BADGE_RENDERER_HPP 1

#include <libfullcircle/common.hpp>
#include <boost/filesystem.hpp>
namespace bfs=boost::filesystem;

namespace fullcircle {
  class BadgeRenderer {
    public:
      typedef std::tr1::shared_ptr<BadgeRenderer> Ptr;
      BadgeRenderer(bfs::path templatefile) 
        : _templatefile(templatefile) {};
      virtual ~BadgeRenderer() {};
      std::string render();

    private:
      BadgeRenderer (const BadgeRenderer& original);
      BadgeRenderer& operator= (const BadgeRenderer& rhs);
      bfs::path _templatefile;
      
  };
  
};


#endif /* LIBFULLCIRCLE_PRINTER_BADGE_RENDERER_HPP */

