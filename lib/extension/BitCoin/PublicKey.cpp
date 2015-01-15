#include "PublicKey.hpp"
#include <QDataStream>

PublicKey::PublicKey() {
}

QString PublicKey::wif() const {
    return "test";
}

QDataStream & operator<<(QDataStream & stream, const PublicKey & key) {

    // put key into this QDataStream

    return stream;
}

QDataStream & operator>>(QDataStream & stream, PublicKey & key) {

    // put key into this QDataStream

    return stream;
}
