#ifndef LIBFULLCIRCLE_FRAME_HPP
#define LIBFULLCIRCLE_FRAME_HPP 1

#include <libfullcircle/common.hpp>
#include <vector>
#include <ostream>

namespace fullcircle {

  typedef struct {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
  } RGB_t;

  class Frame {
    public:
      typedef std::tr1::shared_ptr<Frame> Ptr;
      Frame ( uint16_t x_dim, uint16_t y_dim);
      Frame (Frame& rhs);
      virtual ~Frame() {};
      void set_pixel(
          const uint16_t& x,
          const uint16_t& y,
          const uint16_t& red,
          const uint16_t& green,
          const uint16_t& blue
        );
      void set_pixel(
          const uint16_t& x,
          const uint16_t& y,
          const RGB_t& color
        );
      const RGB_t get_pixel(
          const uint16_t& x,
          const uint16_t& y
        );
      const uint16_t x_dim() { return _x_dim; };
      const uint16_t y_dim() { return _y_dim; };
      void dump_frame(std::ostream& os);

      Frame& operator= (const Frame& rhs);
      bool operator==(Frame &rhs);
      bool operator!=(Frame &rhs);

    private:
      inline void check_coordinates(
          const uint16_t& x,
          const uint16_t& y);
      uint16_t _x_dim;
      uint16_t _y_dim;
      std::vector<RGB_t> _pixels;
  };
};


#endif /* LIBFULLCIRCLE_FRAME_HPP */

