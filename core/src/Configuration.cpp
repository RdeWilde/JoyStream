/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 15 2016
 */

#include <core/Configuration.hpp>
#include <core/Exception.hpp>

#include <libtorrent/create_torrent.hpp>

namespace joystream {
namespace core {
namespace configuration {

/// BuyingPlugin
BuyingPlugin::BuyingPlugin() {}
BuyingPlugin::BuyingPlugin(const protocol_session::BuyingPolicy & policy,
                           const protocol_wire::BuyerTerms & terms)
    : policy(policy)
    , terms(terms) {
}

/// SellingPlugin

SellingPlugin::SellingPlugin() {}
SellingPlugin::SellingPlugin(const protocol_session::SellingPolicy & policy,
                             const protocol_wire::SellerTerms & terms)
    : policy(policy)
    , terms(terms) {
}

/// TorrentPlugin

TorrentPlugin::TorrentPlugin()
    : _mode(protocol_session::SessionMode::not_set) {
}

TorrentPlugin TorrentPlugin::inBuyMode(extension::TorrentPlugin::LibtorrentInteraction interaction,
                                       protocol_session::SessionState state,
                                       const protocol_session::BuyingPolicy & policy,
                                       const protocol_wire::BuyerTerms & terms) {
    TorrentPlugin p;
    p._interaction = interaction;
    p._state = state;
    p._mode = protocol_session::SessionMode::buying;
    p._buying = BuyingPlugin(policy, terms);

    return p;
}

TorrentPlugin TorrentPlugin::inSellMode(extension::TorrentPlugin::LibtorrentInteraction interaction,
                                        protocol_session::SessionState state,
                                        const protocol_session::SellingPolicy & policy,
                                        const protocol_wire::SellerTerms & terms) {
    TorrentPlugin p;
    p._interaction = interaction;
    p._state = state;
    p._mode = protocol_session::SessionMode::selling;
    p._selling = SellingPlugin(policy, terms);

    return p;
}

TorrentPlugin TorrentPlugin::inObserveMode(extension::TorrentPlugin::LibtorrentInteraction interaction,
                                           protocol_session::SessionState state) {
    TorrentPlugin p;
    p._interaction = interaction;
    p._state = state;
    p._mode = protocol_session::SessionMode::observing;

    return p;
}

protocol_session::SessionMode TorrentPlugin::mode() const noexcept {
    return _mode;
}

BuyingPlugin TorrentPlugin::buying() const noexcept {
    return _buying;
}

SellingPlugin TorrentPlugin::selling() const noexcept {
    return _selling;
}

/// Torrent

Torrent::Torrent() {}

Torrent::Torrent(const boost::optional<uint> & uploadLimit,
                 const boost::optional<uint> & downloadLimit,
                 const std::string & name,
                 const std::vector<char> & resumeData,
                 const std::string & savePath,
                 bool pause,
                 const TorrentReference & torrentReference,
                 const TorrentPlugin & plugin)
    : _uploadLimit(uploadLimit)
    , _downloadLimit(downloadLimit)
    , _name(name)
    , _resumeData(resumeData)
    , _savePath(savePath)
    , _pause(pause)
    , _torrentReference(torrentReference)
    , _plugin(plugin) {
}

libtorrent::add_torrent_params Torrent::toAddTorrentParams() const noexcept {

    // Initialize with default values
    libtorrent::add_torrent_params params;

    if(_uploadLimit.is_initialized())
        params.upload_limit = _uploadLimit.value();

    if(_downloadLimit.is_initialized())
        params.download_limit = _downloadLimit.value();

    params.name = _name;
    params.resume_data = _resumeData;
    params.save_path = _savePath;

    // NB: initial flags set in params is
    // default_flags = flag_pinned | flag_update_subscribe | flag_auto_managed | flag_paused | flag_apply_ip_filter

    // Pause, if paused
    if(_pause)
        params.flags += libtorrent::add_torrent_params::flags_t::flag_paused;

    // Remove auto-managing
    params.flags -= libtorrent::add_torrent_params::flags_t::flag_auto_managed;

    // Torrent refernce (usual, and safer, visitor pattern is a bit too heavy)
    switch(_torrentReference.which()) {

        case 0: params.info_hash = boost::get<libtorrent::sha1_hash>(_torrentReference); break;

        case 1: params.url = boost::get<MagnetLink>(_torrentReference).toURI(); break;

        case 2: params.ti = boost::shared_ptr<libtorrent::torrent_info>(new libtorrent::torrent_info(boost::get<libtorrent::torrent_info>(_torrentReference))); break;

        default:
            assert(false);
    }

    return params;
}

/// Controller

Node::Node() {
}

}
}
}
