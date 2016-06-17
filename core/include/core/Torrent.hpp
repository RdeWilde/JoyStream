/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 17 2016
 */

#ifndef JOYSTREAM_CORE_TORRENT_HPP
#define JOYSTREAM_CORE_TORRENT_HPP

namespace joystream {
namespace core {

/** Make sure that there are only const routines and signals on all types **/

/**
class PeerPlugin {

};

class TorrentPlugin {

};
*/

// Torrent
class Torrent : public QObject {

public:

    // streaming routines
    // this can be used to stream into a http daemon, etc.
    // *initiating stream (register a callback)
    // *stopping stream

private:

    //TorrentPlugin _torrentPlugin;

};

}
}

#endif // JOYSTREAM_CORE_TORRENT_HPP
