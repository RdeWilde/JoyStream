#include "RefundSigned.hpp"
#include "MessageType.hpp"

#include <QDataStream>

RefundSigned::RefundSigned() {
}

RefundSigned::RefundSigned(const Signature & sig)
    : _sig(sig) {
}

RefundSigned::RefundSigned(QDataStream & stream, quint8 lengthOfSignature) {

    // Check that signature has valid length
    if(lengthOfSignature > Signature::maxLength)
        throw std::exception("Maximum signature length exceeded.");

    // Read signature
    _sig.readFromStream(stream, lengthOfSignature);
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
