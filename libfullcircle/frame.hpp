#ifndef LIBFULLCIRCLE_FRAME_HPP
#define LIBFULLCIRCLE_FRAME_HPP 1

#include <libfullcircle/common.hpp>
#include <vector>

namespace fullcircle {

  typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
  } RGB_t;

  class Frame {
    public:
      typedef std::tr1::shared_ptr<Frame> Ptr;
      Frame (
          const uint8_t& x_dim, 
          const uint8_t& y_dim) 
        : _x_dim(x_dim)
        , _y_dim(y_dim)
        , _framedata(x_dim * y_dim) {};
      void set_pixel(
          const uint8_t& x,
          const uint8_t& y,
          const uint8_t& red,
          const uint8_t& green,
          const uint8_t& blue
        );
      void set_pixel(
          const uint8_t& x,
          const uint8_t& y,
          const RGB_t& color
        );
      const RGB_t get_pixel(
          const uint8_t& x,
          const uint8_t& y
        );

   
      virtual ~Frame() {};

    private:
      Frame (const Frame& original);
      Frame& operator= (const Frame& rhs);
      inline void check_coordinates(
          const uint8_t& x,
          const uint8_t& y);
      uint8_t _x_dim;
      uint8_t _y_dim;
      std::vector<RGB_t> _framedata;
  };
};
  

#endif /* LIBFULLCIRCLE_FRAME_HPP */

