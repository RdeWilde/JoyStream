/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#include <core/TorrentPlugin.hpp>
#include <core/detail/detail.hpp>
#include <core/PeerPlugin.hpp>
#include <core/Session.hpp>
#include <core/Exception.hpp>

Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)

namespace joystream {
namespace core {

void TorrentPlugin::registerMetaTypes() {
    PeerPlugin::registerMetaTypes();
    Session::registerMetaTypes();
    qRegisterMetaType<libtorrent::tcp::endpoint>();
}

TorrentPlugin::TorrentPlugin(const libtorrent::sha1_hash & infoHash,
                             Session * session,
                             const boost::shared_ptr<extension::Plugin> & plugin)
    : _infoHash(infoHash)
    , _session(session)
    , _plugin(plugin) {
}

TorrentPlugin * TorrentPlugin::create(const extension::status::TorrentPlugin & status,
                                      const boost::shared_ptr<extension::Plugin> & p) {

    TorrentPlugin * plugin = new TorrentPlugin(status.infoHash,
                                               Session::create(status.session),
                                               p);
    //for(auto m : status.peers)
    //    plugin->addPeerPlugin(m.second);

    return plugin;
}

TorrentPlugin::~TorrentPlugin() {

    for(auto it = _peers.cbegin();it != _peers.cend();)
        removePeerPlugin(it++);
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

void TorrentPlugin::toSellMode(const protocol_session::GenerateP2SHKeyPairCallbackHandler & generateKeyPairCallbackHandler,
                         const protocol_session::GenerateReceiveAddressesCallbackHandler &generateReceiveAddressesCallbackHandler,
                         const protocol_session::SellingPolicy & sellingPolicy,
                         const protocol_wire::SellerTerms & terms,
                         const extension::request::SubroutineHandler & handler) {

    _plugin->submit(extension::request::ToSellMode(_infoHash,
                                                   generateKeyPairCallbackHandler,
                                                   generateReceiveAddressesCallbackHandler,
                                                   sellingPolicy,
                                                   terms,
                                                   handler));
}

void TorrentPlugin::toBuyMode(const protocol_session::GenerateP2SHKeyPairCallbackHandler & generateKeyPairCallbackHandler,
                        const protocol_session::GenerateReceiveAddressesCallbackHandler &generateReceiveAddressesCallbackHandler, const protocol_session::GenerateChangeAddressesCallbackHandler &generateChangeAddressesCallbackHandler,
                        const Coin::UnspentOutputSet &funding,
                        const protocol_session::BuyingPolicy & policy,
                        const protocol_wire::BuyerTerms & terms,
                        const extension::request::SubroutineHandler & handler) {

    _plugin->submit(extension::request::ToBuyMode(_infoHash,
                                                  generateKeyPairCallbackHandler,
                                                  generateReceiveAddressesCallbackHandler,
                                                  generateChangeAddressesCallbackHandler,
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

Session * TorrentPlugin::session() const {

    assert(_session.get() != nullptr);

    return _session.get();
}

void TorrentPlugin::addPeerPlugin(const extension::status::PeerPlugin & status) {

    // Ignore if it has already been added
    if(_peers.count(status.endPoint) > 0)
        return;

    // Create peer plugin
    PeerPlugin * plugin = PeerPlugin::create(status);

    // Add to map
    _peers.insert(std::make_pair(status.endPoint, std::unique_ptr<PeerPlugin>(plugin)));

    // announce
    emit peerPluginAdded(plugin);
}

void TorrentPlugin::removePeerPlugin(const libtorrent::tcp::endpoint & endPoint) {

    auto it = _peers.find(endPoint);
    assert(it != _peers.end());

    removePeerPlugin(it);
}

void TorrentPlugin::removePeerPlugin(std::map<libtorrent::tcp::endpoint, std::unique_ptr<PeerPlugin>>::const_iterator it) {

    libtorrent::tcp::endpoint endPoint = it->first;

    // Remove from map
    _peers.erase(it);

    // announce
    emit peerPluginRemoved(endPoint);
}

void TorrentPlugin::update(const extension::status::TorrentPlugin & status) {

    assert(_infoHash == status.infoHash);

    // Session
    _session->update(status.session);

    // Send signal
    emit statusUpdated(status);
}

}
}
