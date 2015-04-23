
#include "NoPayloadMessage.hpp"

quint32 NoPayloadMessage::length() const {
    return 0;
}

void NoPayloadMessage::write(QDataStream & stream) const {
    // Nothing to write
}
