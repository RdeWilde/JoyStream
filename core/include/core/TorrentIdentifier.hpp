/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 21 2016
 */

#ifndef JOYSTREAM_CORE_TORRENTIDENTIFIER_HPP
#define JOYSTREAM_CORE_TORRENTIDENTIFIER_HPP

#include <core/MagnetLink.hpp>

#include <libtorrent/socket.hpp>
#include <libtorrent/torrent_info.hpp>

namespace joystream {
namespace core {

// Represents most complete reference we have to a torrent (hash of info_hash, magnet link, torrent file)
//typedef boost::variant<libtorrent::sha1_hash, MagnetLink, boost::shared_ptr<libtorrent::torrent_info>> TorrentIdentifier;

class TorrentIdentifier {

public:

    enum class Type {
        MagnetLink,
        Hash,
        TorrentFile
    };

    TorrentIdentifier(const libtorrent::sha1_hash &);
    TorrentIdentifier(const MagnetLink &);
    TorrentIdentifier(const boost::shared_ptr<libtorrent::torrent_info> &);
    TorrentIdentifier(const TorrentIdentifier*);

    static TorrentIdentifier fromTorrentFilePath(const std::string &);
    static TorrentIdentifier fromTorrentFileContents(const std::vector<unsigned char>&);
    static TorrentIdentifier fromHashString(const std::string &);
    static TorrentIdentifier fromMagnetLinkString(const std::string &);

    Type type() const noexcept;

    libtorrent::sha1_hash infoHash() const noexcept;

    MagnetLink magnetLink() const noexcept;

    boost::shared_ptr<libtorrent::torrent_info> torrentFile() const noexcept;

private:

    Type _type;

    libtorrent::sha1_hash _infoHash;

    MagnetLink _magnetLink;

    boost::shared_ptr<libtorrent::torrent_info> _torrentFile;
};
}
}
#endif // JOYSTREAM_CORE_TORRENTIDENTIFIER_HPP
