#include "redis.hpp"
#include <iostream>
#include <sstream>

using namespace fullcircle;

Redis::Redis(
    const std::string& server,
    const uint16_t port)
{
  struct timeval timeout = { 1, 500000 }; // 1.5 seconds
  _c = redisConnectWithTimeout(server.c_str(), port, timeout);
  if (_c->err) {
    std::ostringstream oss;
    oss << "Connection error: " << _c->errstr;
    throw CommunicationException(oss.str()); 
  }
}

Redis::~Redis() {
  redisFree(_c);
}

uint32_t Redis::get_unique_number() {
  redisReply* reply;
  uint16_t retval=0;
  // INCR is an atomic counter. See http://redis.io/commands/incr
  std::ostringstream oss;
  oss << "INCR " << REDIS_COUNTER_VARIABLE;
  reply = (redisReply*) redisCommand(_c, oss.str().c_str());
  retval = reply->integer;
  //std::cout << "Current counter is " << reply->integer << std::endl;
  freeReplyObject(reply);
  // Very small window for fuckups here. The server can 
  // probably configured to save on its own.
  reply = (redisReply*) redisCommand(_c, "SAVE");
  //std::cout << "Save command answered: " << reply->str << std::endl;
  freeReplyObject(reply);
  return retval;
}
