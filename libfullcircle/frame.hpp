#ifndef LIBFULLCIRCLE_FRAME_HPP
#define LIBFULLCIRCLE_FRAME_HPP 1

#include <libfullcircle/common.hpp>
#include <vector>
#include <ostream>
#include <boost/multi_array.hpp>


namespace fullcircle {

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
	  void set_pixel(
					 const uint16_t& x,
					 const uint16_t& y,
					 Frame::Ptr mini_frame
					 );
	  void set_pixel_window(
							const uint16_t& x,
							const uint16_t& y,
							Frame::Ptr gigantic_frame
							);
    const RGB_t get_pixel(
        const uint16_t& x,
        const uint16_t& y
        );
	  
    const uint16_t width() { return _width; };
    const uint16_t height() { return _height; };
    void dump_frame(std::ostream& os);
    void fill_whole(const RGB_t& color);
    void fill_gradient_horizontal(const RGB_t& from, const RGB_t& to);
    void fill_gradient_vertical(const RGB_t& from, const RGB_t& to);
    void swap_color(const RGB_t& from, const RGB_t& to);
    bool is_black();

    Frame& operator= (const Frame& rhs);
    bool operator==(Frame &rhs);
    bool operator!=(Frame &rhs);
    Frame::Ptr operator+(Frame::Ptr rhs);

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

