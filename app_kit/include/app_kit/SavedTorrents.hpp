/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

#ifndef JOYSTREAM_APPKIT_NODESTATE_HPP
#define JOYSTREAM_APPKIT_NODESTATE_HPP

#include <core/Node.hpp>
#include <libtorrent/sha1_hash.hpp>

namespace joystream {
namespace appkit {

class SavedTorrentParameters;

class SavedTorrents
{
public:
    typedef std::map<libtorrent::sha1_hash, SavedTorrentParameters> SavedTorrentsMap;

    SavedTorrents();

    // Construct node state from an instance of Node
    SavedTorrents(const std::map<libtorrent::sha1_hash, core::Torrent*>&);

    // Construct node state from json value
    SavedTorrents(const QJsonValue&);

    SavedTorrentsMap::size_type size() const;

    void insert(const SavedTorrentParameters&);

    QJsonValue toJson() const;

    SavedTorrentsMap torrents() const;

private:
    SavedTorrentsMap _torrents;
};

}
}
#endif // JOYSTREAM_APPKIT_NODESTATE_HPP
