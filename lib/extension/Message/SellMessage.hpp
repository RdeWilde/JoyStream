#ifndef SELL_MESSAGE_HPP
#define SELL_MESSAGE_HPP

#include "ExtendedMessagePayload.hpp"

class SellMessage : public ExtendedMessagePayload
{

public:

    // Constructor based on members
    SellMessage(quint32 price); //, quint32 fee, quint32 minimum);

    // Constructor based on raw data
    SellMessage(QDataStream & stream);

    /**
     * Inherited from ExtendedMessagePayload
     */
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

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
