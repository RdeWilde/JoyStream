#ifndef OBSERVE_HPP
#define OBSERVE_HPP

#include "NoPayloadMessage.hpp"

class Observe : public NoPayloadMessage
{
public:
    virtual MessageType messageType() const;
};

#endif // OBSERVE_HPP
