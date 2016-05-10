/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_PLUGIN_MODE_HPP
#define JOYSTREAM_EXTENSION_PLUGIN_MODE_HPP

namespace joystream {
namespace extension {

    // Mode of torrent or peer plugin when it has been started
    enum class PluginMode {
        Observer,
        Seller,
        Buyer
    };

}
}

#endif // JOYSTREAM_EXTENSION_PLUGIN_MODE_HPP
