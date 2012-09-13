#ifndef LIBFULLCIRCLE_PRINTER_BADGE_RENDERER_HPP
#define LIBFULLCIRCLE_PRINTER_BADGE_RENDERER_HPP 1

#include <libfullcircle/common.hpp>
#include <boost/filesystem.hpp>
namespace bfs=boost::filesystem;

namespace fullcircle {
  class BadgeRenderer {
    public:
      typedef boost::shared_ptr<BadgeRenderer> Ptr;
      BadgeRenderer(
          bfs::path templatefile
        ) : _templatefile(templatefile) {};
      virtual ~BadgeRenderer() {};
      std::string render(
          const std::string& color,
          const std::string& number
        );

    private:
      BadgeRenderer (const BadgeRenderer& original);
      BadgeRenderer& operator= (const BadgeRenderer& rhs);
      std::string replace_color(
          const std::string& input, const std::string& color);
      std::string replace_number(
          const std::string& input, const std::string& number);
      std::string replace_umlaute(const std::string& input);
      bfs::path _templatefile;
  };
  
};


#endif /* LIBFULLCIRCLE_PRINTER_BADGE_RENDERER_HPP */

