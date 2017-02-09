#ifndef NETWORKINT_HPP
#define NETWORKINT_HPP

#include <stdint.h>
#include <vector>

#include <common/UCharArray.hpp>

namespace joystream {
namespace protocol_wire {
namespace detail {

bool isLittleEndianMachine();

template<class IntType>
Coin::UCharArray<sizeof(IntType)> Serialize(IntType value);

template<class IntType>
IntType Deserialize(const Coin::UCharArray<sizeof(IntType)> &array);

template<class IntType>
IntType Deserialize(const char* array);

template<class IntType>
IntType hton(IntType);

template<class IntType>
IntType ntoh(IntType);

//// host to network specializations

template<>
uint64_t hton(uint64_t v);

template<>
int64_t hton(int64_t v);

template<>
uint32_t hton(uint32_t v);

template<>
int32_t hton(int32_t v);

template<>
uint16_t hton(uint16_t v);

template<>
int16_t hton(int16_t v);

template<>
uint8_t hton(uint8_t v);

template<>
int8_t hton(int8_t v);

//////// network to host specializations

template<>
uint64_t ntoh(uint64_t v);

template<>
int64_t ntoh(int64_t v);

template<>
uint32_t ntoh(uint32_t v);

template<>
int32_t ntoh(int32_t v);

template<>
uint16_t ntoh(uint16_t v);

template<>
int16_t ntoh(int16_t v);

template<>
uint8_t ntoh(uint8_t v);

template<>
int8_t ntoh(int8_t v);

template<>
Coin::UCharArray<1> Serialize(int8_t value);

template<>
Coin::UCharArray<1> Serialize(uint8_t value);

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
IntType Deserialize(const char* array) {

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

} //detail namespace

template<class IntType>
class NetworkInt : public Coin::UCharArray<sizeof(IntType)> {
public:
    NetworkInt(IntType);
    NetworkInt();

    NetworkInt(const char* raw);

    IntType value() const;

    static std::streamsize size() { return sizeof(IntType); }
};

template<class IntType>
NetworkInt<IntType>::NetworkInt(IntType v)
    : Coin::UCharArray<sizeof(IntType)>(detail::Serialize<IntType>(v)) {
}

template<class IntType>
NetworkInt<IntType>::NetworkInt()
    : Coin::UCharArray<sizeof(IntType)>() {
}

template<class IntType>
IntType NetworkInt<IntType>::value() const {
    return detail::Deserialize<IntType>(*this);
}

template<class IntType>
NetworkInt<IntType>::NetworkInt(const char* raw)
    : Coin::UCharArray<sizeof(IntType)>(reinterpret_cast<const unsigned char*>(raw)) {
}

}
}

#endif // NETWORKINT_HPP
