#ifndef LIBFULLCIRCLE_SEQUENCE_HPP
#define LIBFULLCIRCLE_SEQUENCE_HPP 1

#include <libfullcircle/common.hpp>

namespace fullcircle {
  class Sequence {
    public:
      typedef std::tr1::shared_ptr<Sequence> Ptr;
      Sequence () {};
      virtual ~Sequence() {}; 


    private:
      Sequence (const Sequence& original);
      Sequence& operator= (const Sequence& rhs);
      
  };
};


#endif /* LIBFULLCIRCLE_SEQUENCE_HPP */

