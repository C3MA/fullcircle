#ifndef LIBFULLCIRCLE_COLOR_SCHEME_PLAIN_HPP
#define LIBFULLCIRCLE_COLOR_SCHEME_PLAIN_HPP 1

#include <libfullcircle/common.hpp>

namespace fullcircle {
 
  class ColorSchemePlain : public ColorScheme{
    public:
      typedef std::tr1::shared_ptr<ColorSchemeSmash> Ptr;
      ColorSchemePlain() {};
      virtual ~ColorSchemePlain() {};

      virtual RGB_t get_primary() const {
        fullcircle::RGB_t retval;
        retval.red = 255;
        retval.green = 0;
        retval.blue = 0;
        return retval;
      };
      virtual RGB_t get_secondary() const {
        fullcircle::RGB_t retval;
        retval.red = 0;
        retval.green = 255;
        retval.blue = 0;
        return retval;
      };
      virtual RGB_t get_3rd() const {
        fullcircle::RGB_t retval;
        retval.red = 0;
        retval.green = 0;
        retval.blue = 255;
        return retval;
      };
      virtual RGB_t get_4th() const {
        fullcircle::RGB_t retval;
        retval.red = 0;
        retval.green = 255;
        retval.blue = 255;
        return retval;
      };
      virtual RGB_t get_5th() const {
        fullcircle::RGB_t retval;
        retval.red = 0;
        retval.green = 0;
        retval.blue = 0;
        return retval;
      };
      virtual RGB_t get_background() const {
        return get_5th();
      };



    private:
      ColorSchemePlain (const ColorSchemePlain& original);
      ColorSchemePlain& operator= (const ColorSchemePlain& rhs);

  };

 
};


#endif /* LIBFULLCIRCLE_COLOR_SCHEME_PLAIN_HPP */

