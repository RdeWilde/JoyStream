#include "RefundSigned.hpp"
#include "MessageType.hpp"

#include <QDataStream>

RefundSigned::RefundSigned()
{
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
