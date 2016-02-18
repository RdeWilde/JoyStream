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

    SellerConnection::SellerConnection(const Connection & connection,
                                       SellerClientState clientState,
                                       const SellerTerms & terms,
                                       const joystream::paymentchannel::Payee & payee,
                                       const std::queue<uint32_t> & fullPiecesSent,
                                       const wire::SignRefund & lastSignRefundReceived,
                                       const wire::Payment & lastPaymentReceived,
                                       const wire::RequestFullPiece & lastRequestFullPieceReceived)
        : Connection(connection)
        , _clientState(clientState)
        , _terms(terms)
        , _payee(payee)
        , _fullPiecesSent(fullPiecesSent)
        , _lastSignRefundReceived(lastSignRefundReceived)
        , _lastPaymentReceived(lastPaymentReceived)
        , _lastRequestFullPieceReceived(lastRequestFullPieceReceived) {
    }

    SellerConnection SellerConnection::sellMessageJustSent(const joystream::protocol::Connection & connection,
                                                           const SellerTerms & terms,
                                                           const Coin::KeyPair & payeeContractKeys,
                                                           const Coin::KeyPair & payeePaymentKeys) {
        joystream::paymentchannel::Payee payee(0,
                                               terms.lock(),
                                               terms.price(),
                                               0,
                                               terms.settlementFee(),
                                               0,
                                               Coin::typesafeOutPoint(),
                                               payeeContractKeys,
                                               payeePaymentKeys,
                                               Coin::PublicKey(),
                                               Coin::PublicKey(),
                                               Coin::Signature());

        return SellerConnection(connection,
                                SellerClientState::seller_mode_announced,
                                terms,
                                payee,
                                std::queue<uint32_t>(),
                                wire::SignRefund(),
                                wire::Payment(),
                                wire::RequestFullPiece());
    }

    SellerClientState SellerConnection::clientState() const {
        return _clientState;
    }


    SellerTerms SellerConnection::terms() const {
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
