#include "PrivateKey.hpp"

PrivateKey::PrivateKey()
{
}


QDataStream & operator<<(QDataStream & stream, const PrivateKey & key) {

    // put key into this QDataStream

    return stream;
}

QDataStream & operator>>(QDataStream & stream, PrivateKey & key) {

    // put key into this QDataStream

    return stream;
}
