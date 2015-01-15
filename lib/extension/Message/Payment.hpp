#ifndef PAYMENT_HPP
#define PAYMENT_HPP

#include "ExtendedMessagePayload.hpp"
#include "extension/BitCoin/Signature.hpp"

class Payment : public ExtendedMessagePayload
{
public:
    Payment();

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

private:

    // Buyer payment signature
    Signature _sig;
};

#endif // PAYMENT_HPP
