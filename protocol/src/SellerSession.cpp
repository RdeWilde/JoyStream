/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 11 2016
 */

#include <protocol/SellerSession.hpp>
#include <protocol/BuyerSession.hpp>
#include <protocol/ObserverSession.hpp>

namespace joystream {
namespace protocol {

    SellerSession::SellerSession(const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                 const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                 const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                 const std::map<std::string, SellerConnection> & connections,
                                 const SellerTerms & terms,
                                 uint32_t numberOfPiecesInTorrent)
        : Session(Mode::sell, removedConnectionCallbackHandler, generateKeyPairsCallbackHandler, generateP2PKHAddressesCallbackHandler)
        , _connections(connections)
        , _terms(terms)
        , _numberOfPiecesInTorrent(numberOfPiecesInTorrent) {
    }

    void SellerSession::addConnection(const Connection & connection) {

    }

    void SellerSession::removeConnection(const std::string & name) {

    }

    void SellerSession::processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message) {

    }

    BuyerSession * SellerSession::switchToBuyMode(const BuyerTerms & terms) {
        return 0;
    }

    ObserverSession * SellerSession::switchToObserveMode() {
        return 0;
    }

    void SellerSession::updateTerms(const SellerTerms & terms) {

    }
}
}
