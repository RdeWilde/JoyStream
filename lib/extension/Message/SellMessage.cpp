#include "SellMessage.hpp"
#include "MessageType.hpp"

#include <QDataStream>

SellMessage::SellMessage(quint32 price) //, quint32 fee, quint32 minimum)
    : price_(price)
    /*
    , fee_(fee)
    , minimum_(minimum)*/ {
}

SellMessage::SellMessage(QDataStream & stream) {
    stream >> price_; // >> fee_ >> minimum_;
}

MessageType SellMessage::messageType() const {
    return MessageType::sell;
}

quint32 SellMessage::length() const {
    return sizeof(price_); //3*sizeof(quint32);
}

void SellMessage::write(QDataStream & stream) const {
    stream << price_; // << fee_ << minimum_;
}

quint32 SellMessage::price() const {
    return price_;
}
