#include "BuyMessage.hpp"

MessageType BuyMessage::getMessageType() const {
    return MessageType::buy;
}

quint32 BuyMessage::extendedPayloadLength() const {
    return 0;
}

void BuyMessage::wireForm(const ExtendedMessageIdMapping & mapping, QDataStream & stream) const {

    // Write header
    writeBEP10Header(stream, mapping.id(MessageType::buy));
}
