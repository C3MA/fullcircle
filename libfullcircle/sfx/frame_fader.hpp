#ifndef LIBFULLCIRCLE_SFX_FRAME_FADER_HPP
#define LIBFULLCIRCLE_SFX_FRAME_FADER_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/frame.hpp>
#include <libfullcircle/sequence.hpp>


namespace fullcircle {
  class FrameFader {
    public:
      typedef std::tr1::shared_ptr<FrameFader> Ptr;
      FrameFader (uint16_t num_intermediate_frames, uint16_t fps)
        : _num_intermediate_frames(num_intermediate_frames)
        , _fps(fps)
      {};
      Sequence::Ptr fade(Frame::Ptr from, Frame::Ptr to);
      virtual ~FrameFader() {};

    private:
      FrameFader (const FrameFader& original);
      FrameFader& operator= (const FrameFader& rhs);
      uint16_t _num_intermediate_frames;
      uint16_t _fps;
      
  };
  
};


#endif /* LIBFULLCIRCLE_SFX_FRAME_FADER_HPP */

