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
}

template<class IntType>
class NetworkInt : public Coin::UCharArray<sizeof(IntType)> {
public:
    NetworkInt(IntType);
    NetworkInt();

    IntType value() const;

    static std::streamsize size() { return sizeof(IntType); }
};

}
}

#include "./NetworkInt.cpp"

#endif // NETWORKINT_HPP
