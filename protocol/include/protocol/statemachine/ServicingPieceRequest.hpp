/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 8 2016
 */

#ifndef JOYSTREAM_PROTOCOL_STATE_MACHINE_SERVICING_PIECE_REQUEST_HPP
#define JOYSTREAM_PROTOCOL_STATE_MACHINE_SERVICING_PIECE_REQUEST_HPP

#include <functional>

namespace Coin {
    class Signature;
}

namespace boost {
    template<class T> class shared_array;
}

namespace joystream {
namespace protocol {

    namespace wire {
        class Payment;
        class Sell;
        class Buy;
        class Observe;
    }

    namespace statemachine {

        class Selling;

        class ServicingPieceRequest {

        public:

            enum class State {

                // simple state (initial state):
                piece_requested,

                // simple state:
                waiting_for_payment,

                // pseudo state: terminated
                terminated
            };

            // Classifier calls

            ServicingPieceRequest(Selling * context);

            // Goes to inital state
            //void reset();


            //// Events

            // Message arrival events
            void recv(const wire::Payment & m);
            void recv(const wire::Observe & m);
            void recv(const wire::Sell & m);
            void recv(const wire::Buy & m);

            // Client initiated events
            void pieceLoaded(const boost::shared_array<char> & piece);
            void invalidPieceRequest();
            void invalidPayment(const Coin::Signature & payment);

            // Exit point
            std::function<void()> RequestServiced;

            // Getters
            State state() const;

        private:

            Selling * _context;

            State _state;
        };
    }
}
}

#endif // JOYSTREAM_PROTOCOL_STATE_MACHINE_SERVICING_PIECE_REQUEST_HPP
