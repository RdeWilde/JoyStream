#ifndef JOINING_CONTRACT_HPP
#define JOINING_CONTRACT_HPP

#include "ExtendedMessagePayload.hpp"
#include "extension/BitCoin/PublicKey.hpp"

class JoiningContract : public ExtendedMessagePayload
{
public:
    JoiningContract(const PublicKey & pk);

    // Getter
    PublicKey pk() const;

    /**
     * Virtual methods that subclassing messages have to implement
     */

    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;

private:

    // Key for seller output in contract
    PublicKey _pk;
};

#endif // JOINING_CONTRACT_HPP
