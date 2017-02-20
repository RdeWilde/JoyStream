#include <protocol_wire/NetworkInt.hpp>

#ifdef _WIN32
# include <winsock2.h>
#else
# include <arpa/inet.h>
#endif

#if defined(__linux)
#  include <endian.h>
#elif defined(__FreeBSD__) || defined(__NetBSD__)
#  include <sys/endian.h>
#elif defined(__OpenBSD__)
#  include <sys/types.h>
#  define be64toh(x) betoh64(x)
#endif

// assume windows and osx have defined htonll and ntohll
// define similar macros for remaining unix platforms
#ifndef htonll
#  define htonll(x) htobe64(x)
#endif

#ifndef ntohll
#  define ntohll(x) be64toh(x)
#endif

#include <common/UCharArray.hpp>

namespace joystream {
namespace protocol_wire {
namespace detail {

uint64_t hostToNetworkLongLong(uint64_t value) {
    static const uint32_t one = 1;

    // Check the endianness
    if (*reinterpret_cast<const char*>(&one) == one) {
        // our system is little endian.. convert to big endian
        const uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
        const uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));

        return (static_cast<uint64_t>(low_part) << 32) | high_part;
    } else {
        return value;
    }
}

bool isLittleEndianMachine() {
    static const uint32_t one = 1;
    return (*reinterpret_cast<const char*>(&one) == one);
}

//// host to network specializations

template<>
uint64_t hton(uint64_t v) {
    return htonll(v);
}

template<>
int64_t hton(int64_t v) {
    return htonll(v);
}

template<>
uint32_t hton(uint32_t v) {
    return htonl(v);
}

template<>
int32_t hton(int32_t v) {
    return htonl(v);
}

template<>
uint16_t hton(uint16_t v) {
    return htons(v);
}

template<>
int16_t hton(int16_t v) {
    return htons(v);
}

template<>
uint8_t hton(uint8_t v) {
    return v;
}

template<>
int8_t hton(int8_t v) {
    return v;
}

//////// network to host specializations

template<>
uint64_t ntoh(uint64_t v) {
    return ntohll(v);
}

template<>
int64_t ntoh(int64_t v) {
    return ntohll(v);
}

template<>
uint32_t ntoh(uint32_t v) {
    return ntohl(v);
}

template<>
int32_t ntoh(int32_t v) {
    return ntohl(v);
}

template<>
uint16_t ntoh(uint16_t v) {
    return ntohs(v);
}

template<>
int16_t ntoh(int16_t v) {
    return ntohs(v);
}

template<>
uint8_t ntoh(uint8_t v) {
    return v;
}

template<>
int8_t ntoh(int8_t v) {
    return v;
}

template<>
Coin::UCharArray<1> Serialize(int8_t value) {
    Coin::UCharArray<1> array;
    array.at(0) = value;
    return array;
}

template<>
Coin::UCharArray<1> Serialize(uint8_t value) {
    Coin::UCharArray<1> array;
    array.at(0) = value;
    return array;
}


} // detail namespace
}
}
