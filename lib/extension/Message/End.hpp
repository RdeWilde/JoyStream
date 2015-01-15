#ifndef END_HPP
#define END_HPP

#include "ExtendedMessagePayload.hpp"

class End : public ExtendedMessagePayload
{
public:
    End();

    // Virtual methods that subclassing messages have to implement
    virtual MessageType messageType() const;
    virtual quint32 length() const;
    virtual void write(QDataStream & stream) const;
};

#endif // END_HPP
