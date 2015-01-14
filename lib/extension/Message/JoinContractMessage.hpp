#ifndef JOIN_CONTRACT_MESSAGE_HPP
#define JOIN_CONTRACT_MESSAGE_HPP

#include "ExtendedMessagePayload.hpp"

class JoinContractMessage : public ExtendedMessagePayload
{
public:

    /**
     * Inherited from ExtendedMessagePayload
     */
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;
};

#endif // JOIN_CONTRACT_MESSAGE_HPP
