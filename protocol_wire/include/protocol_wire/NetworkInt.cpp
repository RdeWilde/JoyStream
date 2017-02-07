#include "NetworkInt.hpp"

#include <arpa/inet.h>
#include <common/UCharArray.hpp>

namespace joystream {
namespace protocol_wire {
namespace stream {

//    uint64_t hostToNetworkLongLong(uint64_t value) {
//        static const uint8_t one = 1;

//        // Check the endianness
//        if (*reinterpret_cast<const char*>(&one) == one) {
//            // our system is little endian.. convert to big endian
//            const uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
//            const uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));

//            return (static_cast<uint64_t>(low_part) << 32) | high_part;
//        } else {
//            return value;
//        }
//    }


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

template<class IntType>
Coin::UCharArray<sizeof(IntType)> Serialize(IntType value) {
    // convert the value to network byte order
    IntType beValue = hton<IntType>(value);

    // pointer to int memory location
    IntType *pd=&beValue;

    Coin::UCharArray<sizeof(IntType)> array;

    char *pc = reinterpret_cast<char*>(pd);
    for(size_t i=0; i<sizeof(IntType); i++)
    {
       unsigned char ch = *pc;
       array[i]= ch;
       pc++;
    }

    return array;
}

template<class IntType>
IntType Deserialize(Coin::UCharArray<sizeof(IntType)> &array) {

    IntType beValue;

    // pointer to int array location
    IntType *pd=&beValue;

    char *pc = reinterpret_cast<char*>(pd);
    for(size_t i=0; i<sizeof(IntType); i++)
    {
       *pc = array[i];
       pc++;
    }

    return ntoh<IntType>(beValue);
}


template<class IntType>
NetworkInt<IntType>::NetworkInt(IntType v)
    : Coin::UCharArray<sizeof(IntType)>(Serialize<IntType>(v)) {
}

template<class IntType>
NetworkInt<IntType>::NetworkInt()
    : Coin::UCharArray<sizeof(IntType)>() {
}

template<class IntType>
IntType NetworkInt<IntType>::value() const {
    return Deserialize<IntType>(*this);
}

}
}
}
