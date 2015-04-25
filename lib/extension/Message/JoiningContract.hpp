#ifndef JOINING_CONTRACT_HPP
#define JOINING_CONTRACT_HPP

#include "ExtendedMessagePayload.hpp"
#include "extension/BitCoin/PublicKey.hpp"

class JoiningContract : public ExtendedMessagePayload
{
public:

    // Default constructor
    JoiningContract();

    // Construct from members
    JoiningContract(const PublicKey & contractPk, const PublicKey & finalPk);

    // Constructor based on raw payload
    JoiningContract(QDataStream & stream);

    // Getter
    PublicKey contractPk() const;
    PublicKey finalPk() const;

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

private:

    // Key for seller output in contract
    PublicKey _contractPk;

    // Key for payment to seller
    PublicKey _finalPk;
};

#endif // JOINING_CONTRACT_HPP
