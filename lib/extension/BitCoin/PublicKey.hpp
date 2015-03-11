#ifndef PUBLIC_KEY_HPP
#define PUBLIC_KEY_HPP

#include "FixedBuffer.hpp"

// strlen(03505ea93acd423afcb19c82ca90e1b5b0e950803b661e524949480403f1a678cf) = 66 hex = 33 bytes
typedef FixedBuffer<66> PublicKey;

// qMapLessThanKey is used by QMap to sort keys using < operator of type.
// Since the implementation is already built, there is not way to provide the
// source for operator<(const FixedBuffer<L> & lhs, const FixedBuffer<L> & rhs)
// where it is used, hence it must be implemented explicitly as a function.
bool qMapLessThanKey(const PublicKey & pk1, const PublicKey & pk2);

#endif // PUBLIC_KEY_HPP
