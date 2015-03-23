#include "RefundSigned.hpp"
#include "MessageType.hpp"

#include <QDataStream>

RefundSigned::RefundSigned() {
}

RefundSigned::RefundSigned(const Signature & sig)
    : _sig(sig) {
}

MessageType RefundSigned::messageType() const {
    return MessageType::refund_signed;
}

quint32 RefundSigned::length() const {
    return _sig.length();
}

void RefundSigned::write(QDataStream & stream) const {
    _sig.writeToStream(stream);
}

Signature RefundSigned::sig() const {
    return _sig;
}

void RefundSigned::setSig(const Signature & sig) {
    _sig = sig;
}
