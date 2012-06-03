#ifndef LIBFULLCIRCLE_FRAME_HPP
#define LIBFULLCIRCLE_FRAME_HPP 1

#include <libfullcircle/common.hpp>
#include <vector>
#include <ostream>
#include <boost/multi_array.hpp>


namespace fullcircle {

  typedef struct {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
  } RGB_t;

  class Frame {
    public:
      typedef std::tr1::shared_ptr<Frame> Ptr;
      typedef boost::multi_array<RGB_t, 2> frame_array;
      typedef frame_array::index index;

      Frame ( uint16_t width, uint16_t height);
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
      const uint16_t width() { return _width; };
      const uint16_t height() { return _height; };
      void dump_frame(std::ostream& os);
	  void fillWholeFrame(const RGB_t& color);

      Frame& operator= (const Frame& rhs);
      bool operator==(Frame &rhs);
      bool operator!=(Frame &rhs);

    private:
      inline void check_coordinates(
          const uint16_t& x,
          const uint16_t& y);
      uint16_t _width;
      uint16_t _height;
      frame_array _pixels;
  };
};


#endif /* LIBFULLCIRCLE_FRAME_HPP */

