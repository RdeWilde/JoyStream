#ifndef REFUND_SIGNED_HPP
#define REFUND_SIGNED_HPP

#include "ExtendedMessagePayload.hpp"
#include "extension/BitCoin/Signature.hpp"

class RefundSigned : public ExtendedMessagePayload
{
public:

    // Default constructor
    RefundSigned();

    // Construct from members
    RefundSigned(const Signature & sig);

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

    // Getters and setters
    Signature sig() const;
    void setSig(const Signature & sig);

private:

    // Seller refund signature
    Signature _sig;
};

#endif // REFUND_SIGNED_HPP
