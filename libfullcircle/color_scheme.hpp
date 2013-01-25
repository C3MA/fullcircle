#ifndef LIBFULLCIRCLE_COLOR_SCHEME_HPP
#define LIBFULLCIRCLE_COLOR_SCHEME_HPP 1

#include <libfullcircle/common.hpp>

namespace fullcircle {

  typedef struct {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
  } RGB_t;

  static const RGB_t BLACK = {0,0,0};
  static const RGB_t WHITE = {255,255,255};
  static const RGB_t RED = {255,0,0};
  static const RGB_t YELLOW = {255,255,0};
  static const RGB_t GREEN = {0,255,0};
  static const RGB_t BLUE = {0,0,255};

  // Pure virtual class
  class ColorScheme {
    public:
      typedef boost::shared_ptr<ColorScheme> Ptr;
      ColorScheme() {};
      virtual ~ColorScheme() {};
      virtual RGB_t get_primary() const = 0;
      virtual RGB_t get_secondary() const = 0;
      virtual RGB_t get_3rd() const = 0;
      virtual RGB_t get_4th() const = 0;
      virtual RGB_t get_5th() const = 0;
      virtual RGB_t get_background() const = 0;

    private:
      ColorScheme (const ColorScheme& original);
      ColorScheme& operator= (const ColorScheme& rhs);

  };
};


#endif /* LIBFULLCIRCLE_COLOR_SCHEME_HPP */

