#include "Hash.hpp"

Hash::Hash() {
}

QDataStream & operator<<(QDataStream & stream, const Hash & key) {

    return stream;
}

QDataStream & operator>>(QDataStream & stream, Hash & key) {

    //for(int i = 0;i < HASH_LENGTH;i++)

    return stream;
}
