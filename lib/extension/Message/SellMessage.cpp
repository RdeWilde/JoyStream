#include "SellMessage.hpp"

SellMessage::SellMessage(quint32 price) //, quint32 fee, quint32 minimum)
    : price_(price)
    /*
    , fee_(fee)
    , minimum_(minimum)*/ {
}

SellMessage::SellMessage(QDataStream & extendedPayloadStream) {

    // Read payload fields
    extendedPayloadStream >> price_; // >> fee_ >> minimum_;
}

MessageType SellMessage::getMessageType() const {
    return MessageType::sell;
}

quint32 SellMessage::extendedPayloadLength() const {
    return sizeof(quint32); //3*sizeof(quint32);
}

void SellMessage::wireForm(const ExtendedMessageIdMapping & mapping, QDataStream & stream) const {

    // Write header
    writeBEP10Header(stream, mapping.id(MessageType::sell));

    // Write payload fields
    stream << price_; // << fee_ << minimum_;
}

quint32 SellMessage::price() const
{
    return price_;
}
