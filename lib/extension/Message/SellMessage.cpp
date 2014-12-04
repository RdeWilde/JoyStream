#include "SellMessage.hpp"

SellMessage::SellMessage(quint32 price, quint32 fee, quint32 minimum)
    : price_(price)
    , fee_(fee)
    , minimum_(minimum) {
}

SellMessage::SellMessage(QDataStream & extendedPayloadStream) {

    // Read payload fields
    extendedPayloadStream >> price_ >> fee_ >> minimum_;
}


MessageType SellMessage::getMessageType() const {
    return MessageType::sell;
}

quint32 SellMessage::rawPayloadLength() const {
    return 3*sizeof(quint32);
}

void SellMessage::toRaw(const ExtendedMessageIdMapping & mapping, QDataStream & extendedMessageStream) const {

    // Write extended message id
    extendedMessageStream << mapping.buy();

    // Write payload fields
    extendedMessageStream << price_ << fee_ << minimum_;
}
