#ifndef NETWORKINT_HPP
#define NETWORKINT_HPP

#include <stdint.h>
#include <vector>

#include <common/UCharArray.hpp>

namespace joystream {
namespace protocol_wire {

namespace stream {

template<class IntType>
Coin::UCharArray<sizeof(IntType)> Serialize(IntType value);

template<class IntType>
IntType Deserialize(Coin::UCharArray<sizeof(IntType)> &array);

template<class IntType>
IntType hton(IntType);

template<class IntType>
IntType ntoh(IntType);

template<class IntType>
class NetworkInt : public Coin::UCharArray<sizeof(IntType)> {
public:
    NetworkInt(IntType);
    NetworkInt();

    IntType value() const;

    // Stream operators
    //friend std::ostream & operator<< (std::ostream & stream, const NetworkInt<IntType> & o);
    //friend std::istream & operator>> (std::istream & stream, NetworkInt<IntType> & o);

};

}
}
}
#endif // NETWORKINT_HPP
