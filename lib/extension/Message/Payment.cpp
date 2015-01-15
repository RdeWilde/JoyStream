#include "Payment.hpp"
#include "MessageType.hpp"

#include <QDataStream>

Payment::Payment()
{
}

MessageType Payment::messageType() const {
    return MessageType::payment;
}

quint32 Payment::length() const {
    return Signature::length;
}

void Payment::write(QDataStream & stream) const {
    stream << _sig;
}
