/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUY_MODE_CONNECTION_STATE_HPP
#define JOYSTREAM_PROTOCOL_BUY_MODE_CONNECTION_STATE_HPP

#include <protocol/wire/Sell.hpp>
#include <protocol/wire/JoiningContract.hpp>

namespace joystream {
namespace protocol {

    class BuyModeConnectionState {

    public:

        //
        enum class ClientState {

            // We have not sent any message after extended handshake
            no_joystream_message_sent,

            // We have sent buyer mode message
            buyer_mode_announced,

            // We have sent join_contraact message
            invited_to_contract,

            // We ignored joining_contract message because the contract was full
            ignored_join_contract_from_peer,

            // We have sent sign_refund message
            asked_for_refund_signature,

            // Postponed sendining ready message since not all signatures were ready
            received_valid_refund_signature_and_waiting_for_others,

            // Sent ready message
            // NOT REALLY NEEDED, WE NEVER WAIT FOR ANYTHIN AFTER THIS
            //announced_ready,

            // At least one request message has been sent, for which no piece message
            // has yet been returned
            //waiting_for_requests_to_be_serviced,

            // Not been assigned piece
            needs_to_be_assigned_piece,

            // We have requested a full piece, and we are waiting for it to arrive
            waiting_for_full_piece,

            // We are waiting for libtorrent to fire on_piece_pass() or on_piece_failed()
            // on a full piece which was recently received
            waiting_for_to_validate_and_store_piece
        };

        //
        class PeerState {

        public:

            /**
            // Enumeration of possible states
            // a peer can have when facing buyer
            // and last message sent was
            // 1) welformed
            // 2) state compatible
            enum class LastValidAction {
                no_bitswapr_message_sent, // No message extended message beyond extended handshake has been sent
                mode_announced,
                joined_contract,
                signed_refund,
                sent_valid_piece
            };
            */

            // Bad states
            enum class FailureMode {
                not_failed,
                mode_message_time_out,
                join_contract_time_out,
                refund_signature_time_out,
                refund_incorrectly_signed,
                sent_invalid_piece, // wrong piece, or integrity not intact
            };

            // Default constructor
            PeerState();

            // Constructor from members
            PeerState(//LastValidAction lastAction,
                      FailureMode failureMode,
                      const wire::Sell & lastSellReceived,
                      const wire::JoiningContract & lastJoiningContractReceived);

            // Getters and setters

            //LastValidAction lastAction() const;
            //void setLastAction(LastValidAction lastAction);

            FailureMode failureMode() const;
            void setFailureMode(FailureMode failureMode);

            wire::Sell lastSellReceived() const;
            void setLastSellReceived(const wire::Sell & lastSellReceived);

            wire::JoiningContract lastJoiningContractReceived() const;
            void setLastJoiningContractReceived(const wire::JoiningContract & lastJoiningContractReceived);

        private:

            // Last valid action of peer
            //LastValidAction _lastAction;

            // How peer may have failed
            FailureMode _failureMode;

            // Last seller message
            wire::Sell _lastSellReceived;

            // Last joining contract message
            wire::JoiningContract _lastJoiningContractReceived;
        };

    private:

        //
        ClientState _client;

        //
        PeerState _peer;

    };

}
}

#endif // JOYSTREAM_PROTOCOL_BUY_MODE_CONNECTION_STATE_HPP

