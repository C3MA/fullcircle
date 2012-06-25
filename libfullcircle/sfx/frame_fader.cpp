#include "frame_fader.hpp"

using namespace fullcircle;

Sequence::Ptr FrameFader::fade(Frame::Ptr from, Frame::Ptr to) {
  fullcircle::Sequence::Ptr seq(
      new fullcircle::Sequence(_fps, from->width(), from->height()));
  /**
   * This is a two-step algorithm. First, compute the velocity for each pixel (how
   * fast to fade the three color channels). Then, generate #steps frames
   * according to the precomputed velocity for each pixel and channel.
   */
  uint8_t RED=0;
  uint8_t GREEN=1;
  uint8_t BLUE=2;
  /**
   * Step1: Calculate the velocities. They are stored in a 3dim. array.
   * First dimension: X, second: Y, 3rd: R, G, B values.
   */
  typedef boost::multi_array<int, 3> velocity_array;
  typedef velocity_array::index index;
  velocity_array velocity(
      boost::extents[to->width()][to->height()][3]);
  for (uint16_t y=0; y < to->height(); ++y) {
    for (uint16_t x=0; x < to->width(); ++x) {
      RGB_t dest_color=to->get_pixel(x,y);
      RGB_t last_color=from->get_pixel(x,y);
      velocity[x][y][RED]=(dest_color.red - last_color.red)/(float)_num_intermediate_frames;
      velocity[x][y][GREEN]=(dest_color.green - last_color.green)/(float)_num_intermediate_frames;
      velocity[x][y][BLUE]=(dest_color.blue - last_color.blue)/(float)_num_intermediate_frames;
    }
  }

  // Step2: Apply the velocities
  for( uint16_t s = 1; s < _num_intermediate_frames; ++s) {
    fullcircle::Frame::Ptr generated_frame=
      fullcircle::Frame::Ptr(new fullcircle::Frame(to->width(), to->height()));
    for (uint16_t y=0; y < to->height(); ++y) {
      for (uint16_t x=0; x < to->width(); ++x) {
        RGB_t last_color=from->get_pixel(x,y);
        RGB_t current_color;
        current_color.red=last_color.red + (s * velocity[x][y][RED]);
        current_color.green=last_color.green + (s * velocity[x][y][GREEN]);
        current_color.blue=last_color.blue + (s * velocity[x][y][BLUE]);
        generated_frame->set_pixel(x, y, current_color);
      }
    }
    seq->add_frame(generated_frame);
  }
  // Finally: Add the last frame.
  seq->add_frame(to);
  return seq;
}
