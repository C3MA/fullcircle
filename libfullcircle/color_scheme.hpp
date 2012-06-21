#ifndef LIBFULLCIRCLE_COLOR_SCHEME_HPP
#define LIBFULLCIRCLE_COLOR_SCHEME_HPP 1

namespace fullcircle {

  typedef struct {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
  } RGB_t;


  // Pure virtual class
  class ColorScheme {
    public:
      typedef std::tr1::shared_ptr<ColorScheme> Ptr;
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

