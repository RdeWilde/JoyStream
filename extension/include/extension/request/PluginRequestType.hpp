/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_REQUEST_PLUGIN_REQUEST_TYPE_HPP
#define JOYSTREAM_EXTENSION_REQUEST_PLUGIN_REQUEST_TYPE_HPP


namespace joystream {
namespace extension {
namespace request{

    enum class PluginRequestType {

        StartBuyerTorrentPlugin,
        StartSellerTorrentPlugin,
        StartObserverTorrentPlugin

        //StartTorrentPlugin

    };
}
}
}
#endif // JOYSTREAM_EXTENSION_REQUEST_PLUGIN_REQUEST_TYPE_HPP
