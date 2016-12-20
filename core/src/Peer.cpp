/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 13 2016
 */

#include <core/Peer.hpp>
#include <core/Exception.hpp>

Q_DECLARE_METATYPE(std::string)

namespace joystream {
namespace core {

void Peer::registerMetaTypes() {

    qRegisterMetaType<std::string>();
    //qRegisterMetaType<libtorrent::peer_info>();
}

Peer::Peer(const libtorrent::peer_info & peerInformation,
           PeerPlugin * peerPlugin)
    : _peerInformation(peerInformation)
    , _peerPlugin(peerPlugin) {
}

Peer * Peer::create(const libtorrent::peer_info & peerInformation,
                    const boost::optional<extension::status::PeerPlugin> & status) {

    return new Peer(peerInformation,
                    status.is_initialized() ? new PeerPlugin(status.get()) : nullptr);
}

Peer::~Peer() {
}

libtorrent::peer_info Peer::peerInformation() const noexcept {
    return _peerInformation;
}

PeerPlugin * Peer::peerPlugin() {

    if(peerPluginSet())
        return _peerPlugin.get();
    else
        throw exception::HandleNotSet();
}

void Peer::update(const libtorrent::peer_info & peerInformation) {

    _peerInformation = peerInformation;
}

void Peer::addPeerPlugin(const extension::status::PeerPlugin & status) {

    assert(peerPluginSet());

    // Create peer plugin
    PeerPlugin * plugin = new PeerPlugin(status);

    _peerPlugin.reset(plugin);

    // announce
    emit peerPluginAdded(plugin);
}

void Peer::removePeerPlugin() {

    assert(!peerPluginSet());

    // Drop peer plugin
    _peerPlugin.reset();

    // announce
    emit peerPluginRemoved();
}

}
}
