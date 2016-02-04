/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELL_MODE_CONNECTION_STATE_HPP
#define JOYSTREAM_PROTOCOL_SELL_MODE_CONNECTION_STATE_HPP

#include <protocol/wire/Buy.hpp>
#include <protocol/wire/SignRefund.hpp>
#include <protocol/wire/Payment.hpp>
#include <protocol/wire/RequestFullPiece.hpp>

namespace joystream {
namespace protocol {

    class SellModeConnectionState {

    public:

        //
        enum class ClientState {

            // We have not sent any message after extended handshake
            no_joystream_message_sent,

            // We have sent seller mode message
            seller_mode_announced,

            // We have sent joining contract message
            joined_contract,

            // We ignored contract invitation: Why would we ever do this?
            ignored_contract_invitation,

            // We signed refund
            signed_refund,

            // We ignored signing refund invitation: Why would we ever do this?
            ignored_sign_refund_invitation,

            // Waiting for first request, after the ready message was sent
            awaiting_fullpiece_request_after_ready_announced,

            // Waiting for payment
            awaiting_payment,

            // Waiting for next request, after a payment was made
            awaiting_piece_request_after_payment,

            // We are doing async reading of a piece from disk,
            // which occurs when when a valid full piece request arrives
            reading_piece_from_disk,

            // We are trying to get the last payment received confirmed
            trying_to_claim_last_payment,

            //
            last_payment_confirmed
        };

        //
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
                      const wire::Buy & lastBuyReceived,
                      const wire::SignRefund & lastSignRefundReceived,
                      const wire::Payment & lastPaymentReceived,
                      const wire::RequestFullPiece & lastRequestFullPieceReceived);

            // Getters and setters
            /**
            LastValidAction lastAction() const;
            void setLastAction(LastValidAction lastAction);
            */

            FailureMode failureMode() const;
            void setFailureMode(FailureMode failureMode);

            wire::Buy lastBuyReceived() const;
            void setLastBuyReceived(const wire::Buy & lastBuyReceived);

            wire::SignRefund lastSignRefundReceived() const;
            void setLastSignRefundReceived(const wire::SignRefund & lastSignRefundReceived);

            wire::Payment lastPaymentReceived() const;
            void setLastPaymentReceived(const wire::Payment & lastPaymentReceived);

            wire::RequestFullPiece lastRequestFullPieceReceived() const;
            void setLastRequestFullPieceReceived(const wire::RequestFullPiece & lastRequestFullPieceReceived);

        private:

            // Last valid action of peer
            //LastValidAction _lastAction;

            // How peer may have failed
            FailureMode _failureMode;

            // Message payloads received
            // Is updated so long as it is state compatible,
            // content may be invalid.
            wire::Buy _lastBuyReceived;
            wire::SignRefund _lastSignRefundReceived;
            wire::Payment _lastPaymentReceived; // May be invalid, the valid payment is saved in Payee
            wire::RequestFullPiece _lastRequestFullPieceReceived;
        };

    private:

        // State of client
        ClientState _client;

        // State of peer
        PeerState _peer;

    };

}
}

#endif // JOYSTREAM_PROTOCOL_SELL_MODE_CONNECTION_STATE_HPP

