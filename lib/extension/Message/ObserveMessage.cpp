#include "ObserveMessage.hpp"
#include "MessageType.hpp"

MessageType ObserveMessage::messageType() const {
    return MessageType::observe;
}

quint32 ObserveMessage::length() const {
    return 0;
}

void ObserveMessage::write(QDataStream & stream) const {
}
