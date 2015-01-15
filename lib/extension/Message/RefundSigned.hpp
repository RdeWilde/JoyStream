#ifndef REFUND_SIGNED_HPP
#define REFUND_SIGNED_HPP

#include "ExtendedMessagePayload.hpp"
#include "extension/BitCoin/Signature.hpp"

class RefundSigned : public ExtendedMessagePayload
{
public:
    RefundSigned();

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

private:

    // Seller refund signature
    Signature _sig;
};

#endif // REFUND_SIGNED_HPP
