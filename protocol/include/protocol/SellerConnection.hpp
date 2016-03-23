/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELLER_CONNECTION_HPP
#define JOYSTREAM_PROTOCOL_SELLER_CONNECTION_HPP

#include <protocol/Connection.hpp>
#include <protocol/SellerClientState.hpp>
#include <protocol/SellerTerms.hpp>
#include <protocol/wire/SignRefund.hpp>
#include <protocol/wire/Payment.hpp>
#include <protocol/wire/RequestFullPiece.hpp>
#include <paymentchannel/Payee.hpp>

#include <queue>

namespace joystream {
namespace protocol {

    class SellerConnection : public Connection {

    public:

        SellerConnection();

        SellerConnection(const Connection & connection,
                         SellerClientState clientState,
                         const SellerTerms & terms,
                         const joystream::paymentchannel::Payee & payee,
                         const std::queue<uint32_t> & fullPiecesSent,
                         const wire::SignRefund & lastSignRefundReceived,
                         const wire::Payment & lastPaymentReceived,
                         const wire::RequestFullPiece & lastRequestFullPieceReceived);

        // Construct connection without any prior state, i.e. has never had any message transmitted/received
        static SellerConnection createFreshConnection(const Connection & connection,
                                                      const SellerTerms & terms,
                                                      const Coin::KeyPair & payeeContractKeys,
                                                      const Coin::PubKeyHash & payeeFinalPubKeyHash);

        // Getters
        SellerClientState clientState() const;
        void setClientState(const SellerClientState & clientState);

        SellerTerms terms() const;

        joystream::paymentchannel::Payee payee() const;

        std::queue<uint32_t> fullPiecesSent() const;

        wire::SignRefund lastSignRefundReceived() const;
        void setLastSignRefundReceived(const wire::SignRefund & lastSignRefundReceived);

        wire::Payment lastPaymentReceived() const;
        void setLastPaymentReceived(const wire::Payment & lastPaymentReceived);

        wire::RequestFullPiece lastRequestFullPieceReceived() const;
        void setLastRequestFullPieceReceived(const wire::RequestFullPiece & lastRequestFullPieceReceived);

    private:

        // State of client (us) on this connection
        SellerClientState _clientState;

        // Terms used on this connection
        SellerTerms _terms;

        // Payee side of payment channel
        joystream::paymentchannel::Payee _payee;

        // Index of each piece sent, in the order that it was sent
        std::queue<uint32_t> _fullPiecesSent;

        // Message payloads received
        // Is updated so long as it is state compatible, content may be invalid.
        wire::SignRefund _lastSignRefundReceived;
        wire::Payment _lastPaymentReceived; // May be invalid, the valid payment is saved in Payee
        wire::RequestFullPiece _lastRequestFullPieceReceived;

    };

}
}

#endif // JOYSTREAM_PROTOCOL_SELLER_CONNECTION_HPP
