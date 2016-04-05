/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/SellerConnection.hpp>
#include <protocol/Utilities.hpp>

namespace joystream {
namespace protocol {

    SellerConnection::SellerConnection() {
    }

    SellerConnection::SellerConnection(const Connection & connection,
                                       SellerClientState clientState,
                                       const joystream::wire::SellerTerms & terms,
                                       const joystream::paymentchannel::Payee & payee,
                                       const std::queue<uint32_t> & fullPiecesSent,
                                       const joystream::wire::SignRefund & lastSignRefundReceived,
                                       const joystream::wire::Payment & lastPaymentReceived,
                                       const joystream::wire::RequestFullPiece & lastRequestFullPieceReceived)
        : Connection(connection)
        , _clientState(clientState)
        , _terms(terms)
        , _payee(payee)
        , _fullPiecesSent(fullPiecesSent)
        , _lastSignRefundReceived(lastSignRefundReceived)
        , _lastPaymentReceived(lastPaymentReceived)
        , _lastRequestFullPieceReceived(lastRequestFullPieceReceived) {
    }

    SellerConnection SellerConnection::createFreshConnection(const Connection & connection,
                                                             const joystream::wire::SellerTerms & terms,
                                                             const Coin::KeyPair & payeeContractKeys,
                                                             const Coin::PubKeyHash & payeeFinalPubKeyHash) {

        return SellerConnection(connection,
                                SellerClientState::no_joystream_message_sent,
                                terms,
                                joystream::protocol::utilities::createPayeeForNewSeller(terms.minPrice(),
                                                                                        terms.minLock(),
                                                                                        terms.settlementFee(),
                                                                                        payeeContractKeys,
                                                                                        payeeFinalPubKeyHash),
                                std::queue<uint32_t>(),
                                wire::SignRefund(),
                                wire::Payment(),
                                wire::RequestFullPiece());
    }

    SellerClientState SellerConnection::clientState() const {
        return _clientState;
    }

    void SellerConnection::setClientState(const SellerClientState & clientState) {
        _clientState = clientState;
    }

    joystream::wire::SellerTerms SellerConnection::terms() const {
        return _terms;
    }

    joystream::paymentchannel::Payee SellerConnection::payee() const {
        return _payee;
    }

    std::queue<uint32_t> SellerConnection::fullPiecesSent() const {
        return _fullPiecesSent;
    }

    void SellerConnection::setLastSignRefundReceived(const wire::SignRefund & lastSignRefundReceived) {
        _lastSignRefundReceived = lastSignRefundReceived;
    }

    wire::Payment SellerConnection::lastPaymentReceived() const {
        return _lastPaymentReceived;
    }

    void SellerConnection::setLastPaymentReceived(const wire::Payment & lastPaymentReceived) {
        _lastPaymentReceived = lastPaymentReceived;
    }

    wire::RequestFullPiece SellerConnection::lastRequestFullPieceReceived() const {
        return _lastRequestFullPieceReceived;
    }

    void SellerConnection::setLastRequestFullPieceReceived(const wire::RequestFullPiece & lastRequestFullPieceReceived) {
        _lastRequestFullPieceReceived = lastRequestFullPieceReceived;
    }
}
}
