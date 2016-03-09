/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>

#include <functional>

namespace Coin {
    class typesafeOutPoint;
    class PublicKey;
}

namespace joystream {
namespace protocol {

    namespace wire {
        class Observe;
        class Buy;
        class Sell;
        class JoinContract;
        class Ready;
        class RequestFullPiece;
        class JoiningContract;
    }

    namespace statemachine {

        class CBStateMachine;
        class SellerTerms;
        class BuyerTerms;

        class Sell {

        public:

            enum class State {

                // simple state:
                ready_for_invitation,

                // simple state:
                invited,

                // simple state:
                waiting_to_start,

                // simple state:
                ready_for_piece_request,

                // composite state:
                servicing_piece_request,

                // pseudo state:
                terminated
            };

            //// Classifier call types

            //
            typedef std::function<void()> InvitedToOutdatedContract;

            //
            typedef std::function<void(const Coin::typesafeOutPoint & anchor, int64_t funds, const Coin::PublicKey & contractPk)> InvitedToJoinContract;

            //
            typedef std::function<void(const wire::JoiningContract &)> SendJoiningContract;

            //
            typedef std::function<void(const Coin::typesafeOutPoint &)> ContractIsReady;

            //
            typedef std::function<void(int)> PieceRequested;

            Sell(CBStateMachine * context,
                 const InvitedToOutdatedContract & invitedToOutdatedContract,
                 const InvitedToJoinContract & invitedToJoinContract,
                 const SendJoiningContract & sendJoiningContract,
                 const ContractIsReady & contractIsReady,
                 const PieceRequested & pieceRequested);

            //// Events

            // Message arrival events
            void recv(const wire::Observe & m);
            void recv(const wire::Buy & m);
            void recv(const wire::Sell & m);
            void recv(const wire::JoinContract & m);
            void recv(const wire::Ready & m);
            void recv(const wire::RequestFullPiece & m);

            // Client initiated events
            void clientToObserveMode();
            void clientToSellMode(const SellerTerms & terms, uint32_t index = 0);
            void clientToBuyMode(const BuyerTerms & terms);

            void updateSellTerms(const SellerTerms & terms);
            void joinContract(const Coin::PublicKey & contractPk, const Coin::PublicKey & finalPk);

            // Getters
            State state() const;

        private:

            CBStateMachine * _context;

            State _state;

            // paymentchannel::Payee?

            // Complex state for servicing piece request
            ServicingPieceRequest _servicingPieceRequest;

            // Callbacks for classifier routines
            InvitedToOutdatedContract _invitedToOutdatedContract;
            InvitedToJoinContract _invitedToJoinContract;
            SendJoiningContract _sendJoiningContract;
            ContractIsReady _contractIsReady;
            PieceRequested _pieceRequested;
        };

    }
}
}

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_SELL_HPP
