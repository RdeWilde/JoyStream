/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOIN_CONTRACT_HPP
#define JOIN_CONTRACT_HPP

#include "NoPayloadMessage.hpp"

class JoinContract : public NoPayloadMessage
{
public:

    virtual MessageType messageType() const;
};

#endif // JOIN_CONTRACT_HPP
