#include "envelope.hpp"
#include <cstdlib>
#include <sstream>


using namespace fullcircle;

void Envelope::set_body(const std::string& body) {
  set_body_length(body.size());
  std::memcpy(_data + header_length, body.c_str(), body.size());
  encode_header();
}

std::string Envelope::get_body() {
  std::ostringstream oss;
  oss.write(_data + header_length, _body_length);
  return oss.str();
}

std::string Envelope::str() {
  std::ostringstream oss;
  oss << "Envelope: body has length " << _body_length <<
    ", body is: " << _data + header_length;
  return oss.str();
}

const size_t Envelope::get_body_length() const {
  return _body_length;
}

const size_t Envelope::get_raw_length() const {
  return _body_length + header_length;
}

void Envelope::get_bytes(char* bytes, size_t& length) {
  length = header_length + _body_length;
  std::memcpy(bytes, _data, length);
}

char* Envelope::get_raw_ptr() {
  return _data;
}

char* Envelope::get_body_ptr() {
  return _data + header_length;
}

Envelope::Envelope(char* msg, size_t length) {
  if (length < header_length) {
    throw DataFormatException("Header is too short to contain proper length information");
  }
  std::memcpy(_data, msg, length);
  if (! decode_header()) {
    throw DataFormatException("Cannot decode header");
  }
}

void Envelope::set_body_length(size_t new_length) {
  _body_length = new_length;
  if (_body_length > max_body_length)
    _body_length = max_body_length;
}

bool Envelope::decode_header() {
  char header[header_length + 1] = "";
  std::strncat(header, _data, header_length);
  _body_length = std::atoi(header);
  if (_body_length > max_body_length) {
    _body_length = 0;
    return false;
  }
  return true;
}

void Envelope::encode_header() {
  char header[header_length + 1] = "";
  std::sprintf(header, "%10lu", _body_length);
  std::memcpy(_data, header, header_length);
}
