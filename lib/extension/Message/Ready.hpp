/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef READY_HPP
#define READY_HPP

#include "NoPayloadMessage.hpp"

class Ready : public NoPayloadMessage
{
public:
    virtual MessageType messageType() const;
};

#endif // READY_HPP
