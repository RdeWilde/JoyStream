#ifndef OBSERVE_MESSAGE_HPP
#define OBSERVE_MESSAGE_HPP

#include "ExtendedMessagePayload.hpp"

class ObserveMessage : public ExtendedMessagePayload
{
public:

    /**
     * Inherited from ExtendedMessage
     */
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;
};

#endif // OBSERVE_MESSAGE_HPP
