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
#include <wire/SellerTerms.hpp>
#include <wire/SignRefund.hpp>
#include <wire/Payment.hpp>
#include <wire/RequestFullPiece.hpp>
#include <paymentchannel/Payee.hpp>

#include <queue>

namespace joystream {
namespace protocol {

    class SellerConnection : public Connection {

    public:

        SellerConnection();

        SellerConnection(const Connection & connection,
                         SellerClientState clientState,
                         const joystream::wire::SellerTerms & terms,
                         const joystream::paymentchannel::Payee & payee,
                         const std::queue<uint32_t> & fullPiecesSent,
                         const joystream::wire::SignRefund & lastSignRefundReceived,
                         const joystream::wire::Payment & lastPaymentReceived,
                         const joystream::wire::RequestFullPiece & lastRequestFullPieceReceived);

        // Construct connection without any prior state, i.e. has never had any message transmitted/received
        static SellerConnection createFreshConnection(const Connection & connection,
                                                      const joystream::wire::SellerTerms & terms,
                                                      const Coin::KeyPair & payeeContractKeys,
                                                      const Coin::PubKeyHash & payeeFinalPubKeyHash);

        // Getters
        SellerClientState clientState() const;
        void setClientState(const SellerClientState & clientState);

        joystream::wire::SellerTerms terms() const;

        joystream::paymentchannel::Payee payee() const;

        std::queue<uint32_t> fullPiecesSent() const;

        joystream::wire::SignRefund lastSignRefundReceived() const;
        void setLastSignRefundReceived(const joystream::wire::SignRefund & lastSignRefundReceived);

        joystream::wire::Payment lastPaymentReceived() const;
        void setLastPaymentReceived(const joystream::wire::Payment & lastPaymentReceived);

        joystream::wire::RequestFullPiece lastRequestFullPieceReceived() const;
        void setLastRequestFullPieceReceived(const joystream::wire::RequestFullPiece & lastRequestFullPieceReceived);

    private:

        // State of client (us) on this connection
        SellerClientState _clientState;

        // Terms used on this connection
        joystream::wire::SellerTerms _terms;

        // Payee side of payment channel
        joystream::paymentchannel::Payee _payee;

        // Index of each piece sent, in the order that it was sent
        std::queue<uint32_t> _fullPiecesSent;

        // Message payloads received
        // Is updated so long as it is state compatible, content may be invalid.
        joystream::wire::SignRefund _lastSignRefundReceived;
        joystream::wire::Payment _lastPaymentReceived; // May be invalid, the valid payment is saved in Payee
        joystream::wire::RequestFullPiece _lastRequestFullPieceReceived;

    };

}
}

#endif // JOYSTREAM_PROTOCOL_SELLER_CONNECTION_HPP
