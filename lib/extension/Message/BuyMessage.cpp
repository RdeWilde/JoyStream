#include "BuyMessage.hpp"
#include "MessageType.hpp"

#include <QDataStream>

BuyMessage::BuyMessage(quint32 price, quint32 fee, qint32 btcVersion)
    : _price(price)
    , _fee(fee)
    , _btcVersion(btcVersion) {
}

BuyMessage::BuyMessage(QDataStream & stream) {
    stream >> _price >> _fee >> _btcVersion;
}

MessageType BuyMessage::messageType() const {
    return MessageType::buy;
}

quint32 BuyMessage::length() const {
    return sizeof(_price) + sizeof(_fee) + sizeof(_btcVersion);
}

void BuyMessage::write(QDataStream & stream) const {
    stream << _price << _fee << _btcVersion;
}
