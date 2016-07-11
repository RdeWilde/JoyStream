/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#include <core/MagnetLink.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/socket_io.hpp>
#include <libtorrent/aux_/escape_string.hpp>

namespace joystream {
namespace core {

MagnetLink::MagnetLink() {
}

MagnetLink::MagnetLink(const libtorrent::sha1_hash & infoHash,
                       const std::string & displayName,
                       const std::vector<std::string> & trackers,
                       //const libtorrent::tcp::endpoint & x_pe,
                       const std::vector<std::string> & webSeeds,
                       const std::vector<std::pair<std::string, int> > & dhtNodes)
    : _infoHash(infoHash)
    , _displayName(displayName)
    , _trackers(trackers)
    //, _peers(x_pe)
    , _webSeeds(webSeeds)
    , _dhtNodes(dhtNodes) {
}

MagnetLink MagnetLink::fromURI(const std::string & URI) {

    // Try to parse URI
    libtorrent::add_torrent_params p;
    libtorrent::error_code ec;
    libtorrent::parse_magnet_uri(URI, p, ec);

    // Check if the URI is valid
    if(ec) {
        throw std::runtime_error(ec.message());
    }

    //_peers = p.peers; // Not supported in libtorrent 1.1.0

    return MagnetLink(p.info_hash, p.name, p.trackers, p.url_seeds, p.dht_nodes);
}

std::string MagnetLink::toURI() const noexcept {

    std::string URI;

    URI += "magnet:?xt=urn:btih:" + libtorrent::to_hex(_infoHash.to_string()); // libtorrent::aux::to_hex

    if(!_displayName.empty())
        URI += "&dn=" + escape(_displayName);

    for (auto t : _trackers)
        URI += "&tr=" + escape(t);

    for (auto s : _webSeeds)
        URI += "&ws=" + escape(s);

    for (auto n : _dhtNodes)
        URI += "&dht=" + escape(n.first) + ":" + std::to_string(n.second);

    return URI;
}

libtorrent::sha1_hash MagnetLink::infoHash() const noexcept {
    return _infoHash;
}

std::string MagnetLink::displayName() const noexcept {
    return _displayName;
}

std::vector<std::string> MagnetLink::trackers() const noexcept {
    return _trackers;
}

//libtorrent::tcp::endpoint MagnetLink::peers() const noexcept {
//    return _peers;
//}

std::vector<std::string> MagnetLink::webSeeds() const noexcept {
    return _webSeeds;
}

std::vector<std::pair<std::string, int> > MagnetLink::dhtNodes() const noexcept {
    return _dhtNodes;
}

std::string MagnetLink::escape(const std::string & s) {
    return libtorrent::escape_string(s.c_str(), int(s.length()));
}

}
}
