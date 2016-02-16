/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 30 2016
 */

#ifndef JOYSTREAM_EXTENSION_SELLER_PEER_PLUGIN_PEER_STATE_HPP
#define JOYSTREAM_EXTENSION_SELLER_PEER_PLUGIN_PEER_STATE_HPP

#include <protocol/wire/Buy.hpp>
#include <protocol/wire/SignRefund.hpp>
#include <protocol/wire/Payment.hpp>
#include <protocol/wire/RequestFullPiece.hpp>

namespace joystream {
namespace extension {

    class SellerPeerPluginPeerState {

    public:

        /**
        // Enumeration of possible states
        // a peer can have when facing seller
        // and last message sent was
        // 1) welformed
        // 2) state compatible
        enum class LastValidAction {
            no_bitswapr_message_sent, // No message extended message beyond extended handshake has been sent
            mode_announced,
            sent_contract_invitation,
            requested_refund_signature,
            announced_ready,
            made_request,
            made_payment
        };
        */

        // Bad states
        enum class FailureMode {
            not_failed,

            // Time outs
            mode_message_time_out,
            contract_invitatio_time_out,
            refund_signature_request_time_out,
            announced_ready_time_out,

            payment_incorrectly_signed,

            /**
             * Contract issues
             */

            // Took to long for contract to appear on network
            contract_appearance_time_out,

            // Is this really a failure mode associated with a peer? is not peer's fault.
            //contract_confirmation_time_out,

            // Make more granular later:
            // fee to low
            // to large
            // etc
            contract_invalid
        };

        // Default constructor
        SellerPeerPluginPeerState();

        // Constructor from members
        SellerPeerPluginPeerState(//LastValidAction lastAction,
                  FailureMode failureMode,
                  const joystream::protocol::Buy & lastBuyReceived,
                  const joystream::protocol::SignRefund & lastSignRefundReceived,
                  const joystream::protocol::Payment & lastPaymentReceived,
                  const joystream::protocol::RequestFullPiece & lastRequestFullPieceReceived);

        // Getters and setters
        /**
        LastValidAction lastAction() const;
        void setLastAction(LastValidAction lastAction);
        */

        FailureMode failureMode() const;
        void setFailureMode(FailureMode failureMode);

        joystream::protocol::Buy lastBuyReceived() const;
        void setLastBuyReceived(const joystream::protocol::Buy & lastBuyReceived);

        joystream::protocol::SignRefund lastSignRefundReceived() const;
        void setLastSignRefundReceived(const joystream::protocol::SignRefund & lastSignRefundReceived);

        joystream::protocol::Payment lastPaymentReceived() const;
        void setLastPaymentReceived(const joystream::protocol::Payment & lastPaymentReceived);

        joystream::protocol::RequestFullPiece lastRequestFullPieceReceived() const;
        void setLastRequestFullPieceReceived(const joystream::protocol::RequestFullPiece & lastRequestFullPieceReceived);

    private:

        // Last valid action of peer
        //LastValidAction _lastAction;

        // How peer may have failed
        FailureMode _failureMode;

        // Message payloads received
        // Is updated so long as it is state compatible,
        // content may be invalid.
        joystream::protocol::Buy _lastBuyReceived;
        joystream::protocol::SignRefund _lastSignRefundReceived;
        joystream::protocol::Payment _lastPaymentReceived; // May be invalid, the valid payment is saved in Payee
        joystream::protocol::RequestFullPiece _lastRequestFullPieceReceived;
    };

}
}

#endif // JOYSTREAM_EXTENSION_SELLER_PEER_PLUGIN_PEER_STATE_HPP
