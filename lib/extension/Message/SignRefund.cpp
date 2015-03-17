#include "SignRefund.hpp"
#include "MessageType.hpp"

#include <QDataStream>

SignRefund::SignRefund(const TxId & hash, quint32 index, quint64 value, const PublicKey & pk)
    : _hash(hash)
    , _index(index)
    , _value(value)
    , _pk(pk) {
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
