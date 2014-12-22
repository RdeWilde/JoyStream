#include "PassiveMessage.hpp"

MessageType PassiveMessage::getMessageType() const {
    return MessageType::passive;
}

quint32 PassiveMessage::rawPayloadLength() const {
    return 0;
}

void PassiveMessage::toRaw(const ExtendedMessageIdMapping & mapping, QDataStream & extendedMessageStream) const {

    // Write extended message id
    extendedMessageStream << mapping.id(MessageType::passive);

    // Write payload fields
    // Nothing;
}
