/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 15 2016
 */

#ifndef JOYSTREAM_CORE_CONFIGURATION_HPP
#define JOYSTREAM_CORE_CONFIGURATION_HPP

#include <extension/extension.hpp>
#include <protocol_session/protocol_session.hpp>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_info.hpp>

#include <boost/variant/variant.hpp>

// Configuration classes represent inter-session (on-disk)
// state of the corresponding concept.

namespace joystream {
namespace core {
namespace configuration {

struct BuyingPlugin {

    BuyingPlugin();
    BuyingPlugin(const protocol_session::BuyingPolicy & policy,
                 const protocol_wire::BuyerTerms & terms);

    protocol_session::BuyingPolicy policy;
    protocol_wire::BuyerTerms terms;
};

struct SellingPlugin {

    SellingPlugin();
    SellingPlugin(const protocol_session::SellingPolicy & policy,
                  const protocol_wire::SellerTerms & terms);

    protocol_session::SellingPolicy policy;
    protocol_wire::SellerTerms terms;
};

// Private members not directly exposed,
// in order to ensure, throught he named cosntructors, that fields are always
// coordinated with mode.
class TorrentPlugin {

public:

    TorrentPlugin();

    TorrentPlugin inBuyMode(const protocol_session::BuyingPolicy & policy,
                            const protocol_wire::BuyerTerms & terms);

    TorrentPlugin inSellMode(const protocol_session::SellingPolicy & policy,
                             const protocol_wire::SellerTerms & terms);

    TorrentPlugin inObserveMode();

    /// Getters

    protocol_session::SessionMode mode() const noexcept;

    BuyingPlugin buying() const noexcept;

    SellingPlugin selling() const noexcept;

private:

    protocol_session::SessionMode _mode;

    // _mode == SessionMode::observing, not_set
    // no

    // _mode == SessionMode::buying
    BuyingPlugin _buying;

    // _mode == SessionMode::selling
    SellingPlugin _selling;
};

struct Torrent {

    // Represents most complete reference we have to a torrent (hash of info_hash, magnet link, torrent file)
    typedef boost::variant<libtorrent::sha1_hash, std::string, libtorrent::torrent_info> TorrentReference;

    Torrent();

    Torrent(const boost::optional<uint> & uploadLimit,
            const boost::optional<uint> & downloadLimit,
            const std::string & name,
            const std::vector<char> & resumeData,
            const std::string & savePath,
            bool pause,
            const TorrentReference & torrentReference,
            const TorrentPlugin & plugin);

    libtorrent::add_torrent_params toAddTorrentParams() const noexcept;

private:

    // Total (bytes/second across libtorrent+plugin) upload/download limit.
    // If not set, then unlimited.
    boost::optional<uint> _uploadLimit, _downloadLimit;

    // Display name
    std::string _name;

    // Fast resume data
    std::vector<char> _resumeData;

    // Location to save/load torrent from
    std::string _savePath;

    // Libtorrent pause state (NB: not same as pausing plugin)
    bool _pause;

    // Torrent reference
    TorrentReference _torrentReference;

    // Plugin
    TorrentPlugin _plugin;
};

class Node {

public:

    Node();

    Node(const std::map<libtorrent::sha1_hash, Torrent> & torrents);

private:

    // Torrent configurations <?> should thios sbe here
    std::map<libtorrent::sha1_hash, Torrent> _torrents;

};

}
}
}

#endif // JOYSTREAM_CORE_CONFIGURATION_HPP
