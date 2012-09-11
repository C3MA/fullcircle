#ifndef LIBFULLCIRCLE_NET_ENVELOPE_HPP
#define LIBFULLCIRCLE_NET_ENVELOPE_HPP 1

#include <libfullcircle/common.hpp>
#include <deque>


namespace fullcircle {
  class Envelope {
    public:
      enum { header_length = 10 };
      enum { max_body_length = 1024*1024 };
      typedef std::tr1::shared_ptr<Envelope> Ptr;
      Envelope() : _body_length(0) {};
      Envelope(char* msg, size_t length);
      virtual ~Envelope() {};

      void set_body(const std::string& body);
      std::string get_body();
      const size_t get_body_length() const;
      const size_t get_raw_length() const;
      void get_bytes(char* bytes, size_t& length);
      char* get_raw_ptr();
      char* get_body_ptr();
      void encode_header();
      bool decode_header();
      std::string str();

    private:
      Envelope (const Envelope& original);
      Envelope& operator= (const Envelope& rhs);

      void set_body_length(size_t new_length);

      char _data[header_length + max_body_length];
      size_t _body_length;

  };

  typedef std::deque<Envelope::Ptr> envelope_queue_t;
};


#endif /* LIBFULLCIRCLE_NET_ENVELOPE_HPP */

