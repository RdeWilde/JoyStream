#include "PublicKey.hpp"

bool qMapLessThanKey(const PublicKey & pk1, const PublicKey & pk2) {
   //return pk1.less(pk2); // not working??? why
    return pk1.toString() < pk2.toString();
}

bool operator==(const PublicKey & pk1, const PublicKey & pk2) {
    return pk1.toString() == pk2.toString();
}
