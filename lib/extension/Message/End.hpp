#ifndef END_HPP
#define END_HPP

#include "NoPayloadMessage.hpp"

class End : public NoPayloadMessage
{
public:
    virtual MessageType messageType() const;
};

#endif // END_HPP
