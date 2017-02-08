#include "NetworkInt.hpp"

#include <arpa/inet.h>
#include <common/UCharArray.hpp>

namespace joystream {
namespace protocol_wire {

//    uint64_t hostToNetworkLongLong(uint64_t value) {
//        static const uint32_t one = 1;

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

template<class IntType>
Coin::UCharArray<sizeof(IntType)> Serialize(IntType value) {
    // convert the value to network byte order
    IntType nboInt = hton<IntType>(value);

    // pointer to int memory location
    IntType *pint = &nboInt;

    Coin::UCharArray<sizeof(IntType)> array;

    unsigned char *pc = reinterpret_cast<unsigned char*>(pint);
    for(size_t i = 0; i < sizeof(IntType); i++)
    {
       array[i]= *pc;
       pc++;
    }

    return array;
}

template<class IntType>
IntType Deserialize(const Coin::UCharArray<sizeof(IntType)> &array) {

    IntType nboInt;

    // pointer to int
    IntType *pint = &nboInt;

    unsigned char *pc = reinterpret_cast<unsigned char*>(pint);
    for(size_t i = 0; i < sizeof(IntType); i++)
    {
       *pc = array[i];
       pc++;
    }

    return ntoh<IntType>(nboInt);
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
