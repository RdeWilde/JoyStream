#include "PassiveMessage.hpp"

MessageType PassiveMessage::getMessageType() const {
    return MessageType::passive;
}

quint32 PassiveMessage::extendedPayloadLength() const {
    return 0;
}

void PassiveMessage::wireForm(const ExtendedMessageIdMapping & mapping, QDataStream & stream) const {

    // Write header
    writeBEP10Header(stream, mapping.id(MessageType::passive));
}
