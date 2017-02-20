#include <protocol_wire/char_array_buffer.hpp>
#include <stdexcept>
#include <iostream>

char_array_buffer::char_array_buffer(char *begin, char *end) {
    // end pointer should be one location past the last element in the array

    if(begin == end || begin > end || begin == nullptr || end == nullptr) {
        throw std::runtime_error("stream buffer initialized with invalid pointers");
    }

    setg(begin, begin, end);
    setp(begin, end);
}

char_array_buffer::char_array_buffer(std::vector<char>& buff) {
    // end pointer should be one location past the last element in the array

    char* begin = reinterpret_cast<char*>(buff.data());
    char* end = begin + buff.size();

    setg(begin, begin, end);
    setp(begin, end);
}

std::streambuf::pos_type
char_array_buffer::seekoff(off_type __off, std::ios_base::seekdir __way, std::ios_base::openmode __which) {

  pos_type pos;

  if((__which | std::ios::in) == std::ios::in){
      pos = gseekoff(__off, __way);
  }

  if((__which | std::ios::out) == std::ios::out){
      pos = pseekoff(__off, __way);
  }

  // return absolute position
  return pos;
}

std::streambuf::pos_type
char_array_buffer::gseekoff(off_type __off, std::ios_base::seekdir __way) {

  char_type* newgptr = nullptr;

  switch(__way) {
    case std::ios::cur : {
            newgptr =  gptr() + __off; break;
        }
    case std::ios::beg : {
            newgptr =  eback() + __off; break;
        }
    case std::ios::end : {
            newgptr = egptr() - 1 + __off; break;
        }
  }

  // check we are still within the limits of the buffer
  if(newgptr >= egptr()) { return -1; }
  if(newgptr < eback()) { return -1; }

  // bump the get ptr from current position to new pointer position
  gbump(newgptr - gptr());

  // return new absolute position
  return newgptr - eback();
}


std::streambuf::pos_type
char_array_buffer::pseekoff(off_type __off, std::ios_base::seekdir __way) {

  char_type* newgptr = nullptr;

  switch(__way) {
    case std::ios::cur : {
            newgptr =  pptr() + __off; break;
        }
    case std::ios::beg : {
            newgptr =  pbase() + __off; break;
        }
    case std::ios::end : {
            newgptr = epptr() - 1 + __off; break;
        }
  }

  // check we are still within the limits of the buffer
  if(newgptr >= epptr()) { return -1; }
  if(newgptr < pbase()) { return -1; }

  // bump the put ptr from current position to new pointer position
  pbump(newgptr - pptr());

  // return new absolute position
  return newgptr - pbase();
}

std::streambuf::pos_type
char_array_buffer::seekpos(pos_type __sp, std::ios_base::openmode __which) {
    return seekoff(__sp, std::ios::beg, __which);
}
