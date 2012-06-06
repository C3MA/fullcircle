#include "sprite_io.hpp"
#include <QImage>
#include <QColor>

using namespace fullcircle;

Frame::Ptr SpriteIO::load(const std::string& filename) {
  //std::cout << "Attempting to load " << filename << std::endl;
  QImage* image=new QImage(filename.c_str());
  //std::cout << "Found width: " << image->width()
  //  << ", height: " << image->height() << std::endl;
  Frame::Ptr retval(new Frame(image->width(), image->height()));
  fullcircle::RGB_t pixel_color;
  for( int x = 0; x < image->width(); ++x) {
    for( int y = 0; y < image->height(); ++y) {
      QRgb rgb=image->pixel(x,y);
      pixel_color.red=qRed(rgb);
      pixel_color.green=qGreen(rgb);
      pixel_color.blue=qBlue(rgb);
      retval->set_pixel(x,y,pixel_color);
    }
  }
  delete image;

  return retval;

}
