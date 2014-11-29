#include "BuyMessage.hpp"

Message::TYPE BuyMessage::getMessageType() const {
    return Message::TYPE::buy;
}

quint32 BuyMessage::rawPayloadLength() const {
    return 0;
}

void BuyMessage::toRaw(const ExtendedMessageIdMapping & mapping, QDataStream & extendedMessageStream) const {

    // Write extended message id
    extendedMessageStream << mapping.buy();
}
