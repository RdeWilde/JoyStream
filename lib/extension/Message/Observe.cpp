#include "Observe.hpp"
#include "MessageType.hpp"

MessageType Observe::messageType() const {
    return MessageType::observe;
}

quint32 Observe::length() const {
    return 0;
}

void Observe::write(QDataStream & stream) const {
}
