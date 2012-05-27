#ifndef LIBFULLCIRCLE_SEQUENCE_HPP
#define LIBFULLCIRCLE_SEQUENCE_HPP 1

#include <libfullcircle/common.hpp>
#include <libfullcircle/frame.hpp>
#include <istream>
#include <ostream>

namespace fullcircle {
  class Sequence {
    public:
      typedef std::tr1::shared_ptr<Sequence> Ptr;
      typedef std::vector<Frame::Ptr>::const_iterator const_iterator;
      Sequence (
          const uint8_t& frames_per_second, 
          const uint8_t& x_dim, 
          const uint8_t& y_dim);
      Sequence (std::istream& is);
      virtual ~Sequence() {}; 

      void add_frame(Frame::Ptr frame);
      uint8_t x_dim() const { return _x_dim; };
      uint8_t y_dim() const { return _y_dim; };
      uint32_t size() const { return _frames.size(); };
      void dump(std::ostream& os);
      void save(std::ostream& os,
          const std::string& generator,
          const std::string& version);
      const_iterator begin() const {
        return (_frames.begin()); };
      const_iterator end() const {
        return (_frames.end()); };

    private:
      Sequence (const Sequence& original);
      Sequence& operator= (const Sequence& rhs);
      uint8_t _fps;
      uint8_t _x_dim;
      uint8_t _y_dim;
      std::string _generator_name;
      std::string _generator_version;
      std::vector<Frame::Ptr> _frames;

  };
};


#endif /* LIBFULLCIRCLE_SEQUENCE_HPP */

