/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/Connection.hpp>
#include <protocol/wire/Observe.hpp>
#include <protocol/wire/Buy.hpp>
#include <protocol/wire/Sell.hpp>

namespace joystream {
namespace protocol {

    Connection::Connection() {
    }

    Connection::Connection(const std::string & peerName, PeerModeAnnounced lastModeAnnouncedByPeer, const SendMessageCallbackHandler & sendMessageCallbackHandler)
        : _peerName(peerName)
        , _lastModeAnnouncedByPeer(lastModeAnnouncedByPeer)
        , _sendMessageCallbackHandler(sendMessageCallbackHandler) {
    }

    void Connection::process(const wire::Observe & observe) {
        _lastModeAnnouncedByPeer.setAnnounced(PeerModeAnnounced::ModeAnnounced::observe);
    }

    void Connection::process(const wire::Buy & buy) {
        _lastModeAnnouncedByPeer.setAnnounced(PeerModeAnnounced::ModeAnnounced::buy);
        _lastModeAnnouncedByPeer.setBuyModeTerms(buy.terms());
    }

    void Connection::process(const wire::Sell & sell) {
        _lastModeAnnouncedByPeer.setAnnounced(PeerModeAnnounced::ModeAnnounced::sell);
        _lastModeAnnouncedByPeer.setSellModeTerms(sell.terms());
    }

    std::string Connection::peerName() const {
        return _peerName;
    }

    PeerModeAnnounced Connection::lastModeAnnouncedByPeer() const {
        return _lastModeAnnouncedByPeer;
    }

    Connection::SendMessageCallbackHandler Connection::sendMessageCallbackHandler() const {
        return _sendMessageCallbackHandler;
    }

}
}
