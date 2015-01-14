#include "PublicKey.hpp"
#include <QDataStream>

QDataStream & operator<<(QDataStream & stream, const PublicKey & key) {

    // put key into this QDataStream

    return stream;
}

QDataStream & operator>>(QDataStream & stream, PublicKey & key) {

    // put key into this QDataStream

    return stream;
}

PublicKey::PublicKey() {
}

quint32 PublicKey::length() {
    return -1; // temp
}

bool PublicKey::isValid() const {
    return true;
}
