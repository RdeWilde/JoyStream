/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/SellerConnection.hpp>

namespace joystream {
namespace protocol {

    SellerConnection::SellerConnection() {

    }

    SellerConnection::SellerConnection(const std::string & peerName,
                           PeerModeAnnounced lastModeAnnouncedByPeer,
                           const SendMessageCallbackHandler & sendMessageCallbackHandler,
                           SellerClientState clientState,
                           SellerPeerState peerState,
                           const joystream::paymentchannel::Payee & payee,
                           const std::queue<uint32_t> & fullPiecesSent)
        : Connection(peerName, lastModeAnnouncedByPeer, sendMessageCallbackHandler)
        , _clientState(clientState)
        , _peerState(peerState)
        , _fullPiecesSent(fullPiecesSent) {

    }

    SellerConnection SellerConnection::sellMessageJustSent(const joystream::protocol::Connection & c,
                                                            quint32 lockTime,
                                                            quint64 price,
                                                            const Coin::KeyPair & payeeContractKeys,
                                                            const Coin::KeyPair & payeePaymentKeys) {

        return SellerConnection(c.peerName(),
                              c.lastModeAnnouncedByPeer(),
                              c.sendMessageCallbackHandler(),
                              SellerClientState::seller_mode_announced,
                              SellerPeerState(),
                              joystream::paymentchannel::Payee::unknownPayor(lockTime, price, payeeContractKeys, payeePaymentKeys),
                              std::queue<uint32_t>());
    }

    SellerClientState SellerConnection::clientState() const {
        return _clientState;
    }

    SellerPeerState SellerConnection::peerState() const {
        return _peerState;
    }

    joystream::paymentchannel::Payee SellerConnection::payee() const {
        return _payee;
    }

    std::queue<uint32_t> SellerConnection::fullPiecesSent() const {
        return _fullPiecesSent;
    }

}
}
