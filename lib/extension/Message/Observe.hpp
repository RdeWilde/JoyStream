#ifndef OBSERVE_HPP
#define OBSERVE_HPP

#include "ExtendedMessagePayload.hpp"

class Observe : public ExtendedMessagePayload
{
public:

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;
};

#endif // OBSERVE_HPP
