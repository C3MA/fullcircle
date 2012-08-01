#ifndef LIBFULLCIRCLE_FLOW_MAP_HPP
#define LIBFULLCIRCLE_FLOW_MAP_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/frame.hpp>

namespace fullcircle {

  class FlowMap {
    public:
      typedef std::tr1::shared_ptr<FlowMap> Ptr;
      FlowMap();
	  
	  void init(std::string hash, uint16_t width, uint16_t height); // generate hill profile from a hash
	  void init(Frame::Ptr hills);
	  Frame::Ptr get_next();
	  bool has_changed();
	  
      virtual ~FlowMap() {};

    private:
      FlowMap (const FlowMap& original);
      FlowMap& operator= (const FlowMap& rhs);
      
	  
	  Frame::Ptr _hills;
	  Frame::Ptr _actualColoredFrame;
	  Frame::Ptr _oldColoredFrame;

  };
};


#endif /* LIBFULLCIRCLE_PERLIN_NOISE_HPP */

