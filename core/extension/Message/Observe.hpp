/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef OBSERVE_HPP
#define OBSERVE_HPP

#include "NoPayloadMessage.hpp"

class Observe : public NoPayloadMessage
{
public:
    virtual MessageType messageType() const;
};

#endif // OBSERVE_HPP
