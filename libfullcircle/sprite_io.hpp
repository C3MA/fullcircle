#ifndef LIBFULLCIRCLE_SPRITE_IO_HPP
#define LIBFULLCIRCLE_SPRITE_IO_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/frame.hpp>

namespace fullcircle {
  class SpriteIO {
    public:
      typedef boost::shared_ptr<SpriteIO> Ptr;
      SpriteIO() {};
      virtual ~SpriteIO() {};
      Frame::Ptr load(const std::string& filename);

    private:
      SpriteIO (const SpriteIO& original);
      SpriteIO& operator= (const SpriteIO& rhs);
      
  };
};


#endif /* LIBFULLCIRCLE_SPRITE_IO_HPP */

