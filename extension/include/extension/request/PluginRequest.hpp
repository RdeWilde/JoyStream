/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_REQUEST_PLUGIN_REQUEST_HPP
#define JOYSTREAM_EXTENSION_REQUEST_PLUGIN_REQUEST_HPP

#include <extension/request/PluginRequestType.hpp>

namespace joystream {
namespace extension {
namespace request {

    class PluginRequest {

    public:

        // Returns request type
        virtual PluginRequestType getPluginRequestType() const = 0;

    };

}
}
}

#endif // JOYSTREAM_EXTENSION_REQUEST_PLUGIN_REQUEST_HPP
