/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELL_PEER_STATE_HPP
#define JOYSTREAM_PROTOCOL_SELL_PEER_STATE_HPP

#include <protocol/wire/Buy.hpp>
#include <protocol/wire/SignRefund.hpp>
#include <protocol/wire/Payment.hpp>
#include <protocol/wire/RequestFullPiece.hpp>

namespace joystream {
namespace protocol {
namespace sell {

    class PeerState {

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
        PeerState();

        // Constructor from members
        PeerState(//LastValidAction lastAction,
                  FailureMode failureMode,
                  const joystream::protocol::wire::SignRefund & lastSignRefundReceived,
                  const joystream::protocol::wire::Payment & lastPaymentReceived,
                  const joystream::protocol::wire::RequestFullPiece & lastRequestFullPieceReceived);

        // Getters and setters
        /**
        LastValidAction lastAction() const;
        void setLastAction(LastValidAction lastAction);
        */

        FailureMode failureMode() const;
        void setFailureMode(FailureMode failureMode);

        joystream::protocol::wire::SignRefund lastSignRefundReceived() const;
        void setLastSignRefundReceived(const joystream::protocol::wire::SignRefund & lastSignRefundReceived);

        joystream::protocol::wire::Payment lastPaymentReceived() const;
        void setLastPaymentReceived(const joystream::protocol::wire::Payment & lastPaymentReceived);

        joystream::protocol::wire::RequestFullPiece lastRequestFullPieceReceived() const;
        void setLastRequestFullPieceReceived(const joystream::protocol::wire::RequestFullPiece & lastRequestFullPieceReceived);

    private:

        // Last valid action of peer
        //LastValidAction _lastAction;

        // How peer may have failed
        FailureMode _failureMode;

        // Message payloads received
        // Is updated so long as it is state compatible, content may be invalid.
        joystream::protocol::wire::SignRefund _lastSignRefundReceived;
        joystream::protocol::wire::Payment _lastPaymentReceived; // May be invalid, the valid payment is saved in Payee
        joystream::protocol::wire::RequestFullPiece _lastRequestFullPieceReceived;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_SELL_PEER_STATE_HPP

