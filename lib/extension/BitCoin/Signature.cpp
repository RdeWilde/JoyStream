#include "Signature.hpp"
#include <QDataStream>

Signature::Signature() {
}

Signature::Signature(const Signature& signature) {
}

Signature::Signature(const QString & signature) {

}

Signature & Signature::operator=(const Signature& signature) {

    return *this;
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
