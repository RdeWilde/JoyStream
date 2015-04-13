#include "SignRefund.hpp"
#include "MessageType.hpp"

#include <QDataStream>

SignRefund::SignRefund() {
}

SignRefund::SignRefund(const TxId & hash, quint32 index, quint64 value, const PublicKey & pk)
    : _hash(hash)
    , _index(index)
    , _value(value)
    , _pk(pk) {
}

SignRefund::SignRefund(QDataStream & stream) {
    stream >> _hash >> _index >> _value >> _pk;
}

MessageType SignRefund::messageType() const {
    return MessageType::sign_refund;
}

quint32 SignRefund::length() const {
    return TxId::length + sizeof(quint32) + sizeof(quint64) + PublicKey::length;
}

void SignRefund::write(QDataStream & stream) const {
    stream << _hash << _index << _value << _pk;
}

TxId SignRefund::hash() const {
    return _hash;
}

void SignRefund::setHash(const TxId & hash) {
    _hash = hash;
}

quint32 SignRefund::index() const {
    return _index;
}

void SignRefund::setIndex(quint32 index) {
    _index = index;
}

quint64 SignRefund::value() const {
    return _value;
}

void SignRefund::setValue(quint64 value) {
    _value = value;
}

PublicKey SignRefund::pk() const {
    return _pk;
}

void SignRefund::setPk(const PublicKey & pk) {
    _pk = pk;
}
