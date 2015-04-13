#ifndef READY_HPP
#define READY_HPP

#include "NoPayloadMessage.hpp"

class Ready : public NoPayloadMessage
{
public:
    virtual MessageType messageType() const;
};

#endif // READY_HPP
