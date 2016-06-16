/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 16 2016
 */

#ifndef JOYSTREAM_CORE_VIEW_MODEL_HPP
#define JOYSTREAM_CORE_VIEW_MODEL_HPP

namespace joystream {
namespace core {
namespace viewmodel {

class PeerPlugin {

};

class TorrentPlugin {

};

class Torrent {

    // streaming routines
    // this can be used to stream into a http daemon, etc.
    // *initiating stream (register a callback)
    // *stopping stream

private:

    TorrentPlugin _torrentPlugin;

};

}
}
}

#endif // JOYSTREAM_CORE_VIEW_MODEL_HPP
