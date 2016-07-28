/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#ifndef JOYSTREAM_CORE_MAGNETLINK_HPP
#define JOYSTREAM_CORE_MAGNETLINK_HPP

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/socket.hpp>

namespace joystream {
namespace core {

class MagnetLink {

    /**
    BEP9 (http://bittorrent.org/beps/bep_0009.html)

    The magnet URI format is:

    magnet:?xt=urn:btih:<info-hash>&dn=<name>&tr=<tracker-url>&x.pe=<peer-address>

    <info-hash>
    Is the info-hash hex encoded, for a total of 40 characters. For compatability
    with existing links in the wild, clients should also support the 32 character
    base32 encoded info-hash.

    <peer-address>
    A peer address expressed as hostname:port, ipv4-literal:port or [ipv6-literal]:port.
    This parameter can be included to initiate a direct metadata transfer between two
    clients while reducing the need for external peer sources. It should only be included
    if the client can discover its public IP address and determine its reachability.
    Note: Since no URI scheme identifier has been allocated for bittorrent xs= is not used
    for this purpose.

    xt is the only mandatory parameter.

    dn is the display name that may be used by the client to display while waiting for metadata.

    tr is a tracker url, if there is one. If there are multiple trackers, multiple tr entries may be included.

    The same applies for x.pe entries.

    dn, tr and x.pe are all optional.
    */

public:

    MagnetLink();

    MagnetLink(const libtorrent::sha1_hash & infoHash,
               const std::string & displayName,
               const std::vector<std::string> & trackers,
               //const std::vector<libtorrent::tcp::endpoint> & peers,
               const std::vector<std::string> & webSeeds,
               const std::vector<std::pair<std::string, int> > & dhtNodes);

    // Parse from URI
    static MagnetLink fromURI(const std::string & URI);

    // To the URI
    std::string toURI() const noexcept;

    /// Standard BEP9 fields

    // Info hash
    libtorrent::sha1_hash infoHash() const noexcept;

    // Display name
    std::string displayName() const noexcept;

    // Tracker
    std::vector<std::string> trackers() const noexcept;

    // Peer address
    //std::vector<libtorrent::tcp::endpoint> peers() const noexcept;

    /// Non-standard fields supported by libtorrent

    // Web-seed
    std::vector<std::string> webSeeds() const noexcept;

    // DHT nodes
    std::vector<std::pair<std::string, int> > dhtNodes() const noexcept;

private:

    // Info hash
    libtorrent::sha1_hash _infoHash;

    // Display name
    std::string _displayName;

    // Tracker
    std::vector<std::string> _trackers;

    // Peer address
    // Not supported in libtorrent 1.1.0
    //std::vector<libtorrent::tcp::endpoint> _peers;

    /// Non-standard fields supported by libtorrent

    // Web-seed
    std::vector<std::string> _webSeeds;

    // DHT nodes (host may be name or IP in libtorrent)
    std::vector<std::pair<std::string, int> > _dhtNodes;

    // Escape string to be used in URI
    static std::string escape(const std::string & s);
};

}
}

#endif // JOYSTREAM_CORE_MAGNETLINK_HPP
