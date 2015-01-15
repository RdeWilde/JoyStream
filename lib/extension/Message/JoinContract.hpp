#ifndef JOIN_CONTRACT_HPP
#define JOIN_CONTRACT_HPP

#include "ExtendedMessagePayload.hpp"

class JoinContract : public ExtendedMessagePayload
{
public:

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;
};

#endif // JOIN_CONTRACT_HPP
