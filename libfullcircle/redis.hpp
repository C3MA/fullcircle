#ifndef LIBFULLCIRCLE_REDIS_HPP
#define LIBFULLCIRCLE_REDIS_HPP 1

#include <libfullcircle/common.hpp>
#include <hiredis/hiredis.h>

namespace fullcircle {
  static const std::string REDIS_COUNTER_VARIABLE("counter");
  class Redis {
    public:
      typedef boost::shared_ptr<Redis> Ptr;
      Redis(const std::string& server,
          const uint16_t port);
      virtual ~Redis(); 
      uint32_t get_unique_number();

    private:
      Redis (const Redis& original);
      Redis& operator= (const Redis& rhs);
      redisContext* _c;
  };
};


#endif /* LIBFULLCIRCLE_REDIS_HPP */

