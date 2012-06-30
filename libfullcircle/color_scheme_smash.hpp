#ifndef LIBFULLCIRCLE_COLOR_SCHEME_SMASH_HPP
#define LIBFULLCIRCLE_COLOR_SCHEME_SMASH_HPP 1

#include <libfullcircle/common.hpp>

namespace fullcircle {


  /***
   * Taken from http://kuler.adobe.com/#themeID/1941050
   */

  class ColorSchemeSmash : public ColorScheme{
    public:
      typedef std::tr1::shared_ptr<ColorSchemeSmash> Ptr;
      ColorSchemeSmash() {};
      virtual ~ColorSchemeSmash() {};

      virtual RGB_t get_primary() const {
        fullcircle::RGB_t retval;
        retval.red = 217;
        retval.green = 205;
        retval.blue = 145;
        return retval;
      };
      virtual RGB_t get_secondary() const {
        fullcircle::RGB_t retval;
        retval.red = 217;
        retval.green = 159;
        retval.blue = 108;
        return retval;
      };
      virtual RGB_t get_3rd() const {
        fullcircle::RGB_t retval;
        retval.red = 191;
        retval.green = 96;
        retval.blue = 75;
        return retval;
      };
      virtual RGB_t get_4th() const {
        fullcircle::RGB_t retval;
        retval.red = 166;
        retval.green = 60;
        retval.blue = 60;
        return retval;
      };
      virtual RGB_t get_5th() const {
        fullcircle::RGB_t retval;
        retval.red = 64;
        retval.green = 20;
        retval.blue = 44;
        return retval;
      };
      virtual RGB_t get_background() const {
        return get_5th();
      };



    private:
      ColorSchemeSmash (const ColorSchemeSmash& original);
      ColorSchemeSmash& operator= (const ColorSchemeSmash& rhs);

  };

};


#endif /* LIBFULLCIRCLE_COLOR_SCHEME_SMASH_HPP */

