#ifndef LIBFULLCIRCLE_FLOW_MAP_HPP
#define LIBFULLCIRCLE_FLOW_MAP_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/frame.hpp>

namespace fullcircle {
	
  static const int32_t MAXIMUM = 1024;
	
  class FlowMap {
    public:
      typedef std::tr1::shared_ptr<FlowMap> Ptr;
      FlowMap();
	  
	  void init(std::string hash, uint16_t width, uint16_t height); // generate hill profile from a hash
	  void init(Frame::Ptr hills);
	  void start_points(Frame::Ptr start);
	  Frame::Ptr get_next();
	  bool has_changed();
	  
      virtual ~FlowMap() {};

    private:
      FlowMap (const FlowMap& original);
      FlowMap& operator= (const FlowMap& rhs);
      
	  
	  Frame::Ptr _hills;
	  Frame::Ptr _actualColoredFrame;
	  Frame::Ptr _oldColoredFrame;
	  
	  uint32_t calc_height(Frame::Ptr frame, int32_t x, int32_t y);
  };
};


#endif /* LIBFULLCIRCLE_PERLIN_NOISE_HPP */

