#ifndef JOIN_CONTRACT_HPP
#define JOIN_CONTRACT_HPP

#include "NoPayloadMessage.hpp"

class JoinContract : public NoPayloadMessage
{
public:

    virtual MessageType messageType() const;
};

#endif // JOIN_CONTRACT_HPP
