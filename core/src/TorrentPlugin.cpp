/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#include <core/TorrentPlugin.hpp>
#include <core/detail/detail.hpp>

namespace joystream {
namespace core {

TorrentPlugin::TorrentPlugin(const extension::status::TorrentPlugin & status,
                             const boost::shared_ptr<extension::Plugin> & plugin)
    : _infoHash(status.infoHash)
    , _session(new core::Session(status.session))
    , _plugin(plugin) {

    // Create peers
    for(auto m : status.peers)
        addPeerPlugin(m.second);
}

void TorrentPlugin::start(const extension::request::SubroutineHandler & handler) {
    _plugin->submit(extension::request::Start(_infoHash, handler));
}

void TorrentPlugin::stop(const extension::request::SubroutineHandler & handler) {
    _plugin->submit(extension::request::Stop(_infoHash, handler));
}

void TorrentPlugin::pause(const extension::request::SubroutineHandler & handler) {
   _plugin->submit(extension::request::Pause(_infoHash, handler));
}

void TorrentPlugin::updateTerms(const protocol_wire::BuyerTerms & terms,
                          const extension::request::SubroutineHandler & handler) {
    _plugin->submit(extension::request::UpdateBuyerTerms(_infoHash, terms, handler));
}

void TorrentPlugin::updateTerms(const protocol_wire::SellerTerms & terms,
                          const extension::request::SubroutineHandler & handler) {
    _plugin->submit(extension::request::UpdateSellerTerms(_infoHash, terms, handler));
}

void TorrentPlugin::toObserveMode(const extension::request::SubroutineHandler & handler) {
    _plugin->submit(extension::request::ToObserveMode(_infoHash, handler));
}

void TorrentPlugin::toSellMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                         const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                         const protocol_session::SellingPolicy & sellingPolicy,
                         const protocol_wire::SellerTerms & terms,
                         const extension::request::SubroutineHandler & handler) {

    _plugin->submit(extension::request::ToSellMode(_infoHash,
                                                   generateKeyPairsCallbackHandler,
                                                   generateP2PKHAddressesCallbackHandler,
                                                   sellingPolicy,
                                                   terms,
                                                   handler));
}

void TorrentPlugin::toBuyMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                        const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                        const Coin::UnspentP2PKHOutput & funding,
                        const protocol_session::BuyingPolicy & policy,
                        const protocol_wire::BuyerTerms & terms,
                        const extension::request::SubroutineHandler & handler) {

    _plugin->submit(extension::request::ToBuyMode(_infoHash,
                                                  generateKeyPairsCallbackHandler,
                                                  generateP2PKHAddressesCallbackHandler,
                                                  funding,
                                                  policy,
                                                  terms,
                                                  handler));
}

libtorrent::sha1_hash TorrentPlugin::infoHash() const noexcept {
    return _infoHash;
}

std::map<libtorrent::tcp::endpoint, PeerPlugin *> TorrentPlugin::peers() const noexcept {
    return detail::getRawMap<libtorrent::tcp::endpoint, PeerPlugin>(_peers);
}

void TorrentPlugin::addPeerPlugin(const extension::status::PeerPlugin & status) {

    // Ignore if it has already been added
    if(_peers.count(status.endPoint) > 0)
        return;

    // Create peer plugin
    PeerPlugin * plugin = new PeerPlugin(status);

    // Add to map
    _peers.insert(std::make_pair(status.endPoint, std::unique_ptr<PeerPlugin>(plugin)));

    // announce
    emit peerPluginAdded(plugin);
}

void TorrentPlugin::removePeerPlugin(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _peers.find(endPoint);

    // Ignore if it is already gone
    if(it == _peers.cend())
        return;

    // Remove from map
    _peers.erase(it);

    // announce
    emit peerPluginRemoved(endPoint);
}

void TorrentPlugin::update(const extension::status::TorrentPlugin & status) {

    assert(_infoHash == status.infoHash);

    /// Update peer plugins

    // for each peer with a status
    for(auto p: status.peers) {

        auto it = _peers.find(p.first);

        // if peer is present, then update
        if(it != _peers.cend())
            it->second->update(p.second);
        else // otherwise add
            addPeerPlugin(p.second);

    }

    // for each exisiting peer
    for(auto & p: _peers) {

        // if there is no status for it, then remove
        if(status.peers.count(p.first) == 0)
            removePeerPlugin(p.first);
    }

    // Session
    _session->update(status.session);
}

}
}
