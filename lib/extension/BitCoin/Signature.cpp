#include "Signature.hpp"
#include <QDataStream>

Signature::Signature() {
}

quint32 Signature::length() {
    return -1; // temp
}

bool Signature::isValid(const PublicKey & key) const {
    return true;
}

QDataStream & operator<<(QDataStream & stream, const Signature & key) {
    // put key into this QDataStream
    return stream;
}

QDataStream & operator>>(QDataStream & stream, Signature & key) {
    // put key into this QDataStream
    return stream;
}
