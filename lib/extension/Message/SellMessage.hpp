#ifndef SELL_MESSAGE_HPP
#define SELL_MESSAGE_HPP

#include "ExtendedMessage.hpp"

class SellMessage : public ExtendedMessage
{

public:

    // Constructor based on members
    SellMessage(quint32 price); //, quint32 fee, quint32 minimum);

    // Constructor based on raw data
    SellMessage(QDataStream & extendedPayloadStream);

    /**
     * Inherited from ExtendedMessage
     */
    MessageType getMessageType() const;
    quint32 extendedPayloadLength() const;
    void wireForm(const ExtendedMessageIdMapping & mapping, QDataStream & stream) const;

    // Getters
    quint32 price() const;

private:

    // Unit price
    quint32 price_;

    /*
    // Fee used
    quint32 fee_;

    // Minimum bond required
    quint32 minimum_;
    */

};

#endif // SELL_MESSAGE_HPP
