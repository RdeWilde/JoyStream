#include "PublicKey.hpp"

bool qMapLessThanKey(const PublicKey & pk1, const PublicKey & pk2) {
   return pk1.less(pk2);
}
