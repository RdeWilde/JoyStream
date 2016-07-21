/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 21 2016
 */

#include <core/TorrentIdentifier.hpp>

namespace joystream {
namespace core {

TorrentIdentifier::TorrentIdentifier(const libtorrent::sha1_hash & infoHash)
    : _type(Type::Hash)
    , _infoHash(infoHash) {
}

TorrentIdentifier::TorrentIdentifier(const MagnetLink & magnetLink)
    : _type(Type::MagnetLink)
    , _magnetLink(magnetLink) {
}

TorrentIdentifier::TorrentIdentifier(const boost::shared_ptr<libtorrent::torrent_info> & torrentFile)
    : _type(Type::TorrentFile)
    , _torrentFile(torrentFile) {
}

TorrentIdentifier::Type TorrentIdentifier::type() const noexcept {
    return _type;
}

libtorrent::sha1_hash TorrentIdentifier::infoHash() const noexcept {
    return _infoHash;
}

MagnetLink TorrentIdentifier::magnetLink() const noexcept {
    return _magnetLink;
}

boost::shared_ptr<libtorrent::torrent_info> TorrentIdentifier::torrentFile() const noexcept {
    return _torrentFile;
}

}
}
