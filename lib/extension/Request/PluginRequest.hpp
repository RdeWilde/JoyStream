/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PLUGIN_REQUEST_HPP
#define PLUGIN_REQUEST_HPP

#include "PluginRequestType.hpp"

class PluginRequest
{
public:

    // Returns request type
    virtual PluginRequestType getPluginRequestType() const = 0;

};

#endif // PLUGIN_REQUEST_HPP
