/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELLING_HPP
#define JOYSTREAM_PROTOCOL_SELLING_HPP

#include <protocol_wire/SellerTerms.hpp>

namespace Coin {
    class typesafeOutPoint;
    class Signature;
}

namespace joystream {
namespace protocol_wire {
    class ContractInvitation;
}
namespace protocol_session {

    namespace detail {

        template <class ConnectionIdType>
        struct SessionCoreImpl;
    }

    template <class ConnectionIdType>
    class Selling {

    public:

        Selling(detail::SessionCoreImpl<ConnectionIdType> *);

        // Time out processing hook
        // NB: Later give some indication of how to set timescale for this call
        void tick();

        //// Connection events (ex-post)

        void connectionAdded(const ConnectionIdType &);
        void connectionRemoved(const ConnectionIdType &);

        //// Handling callbacks from connections

        void invitedToOutdatedContract(const ConnectionIdType &);
        void invitedToJoinContract(const ConnectionIdType &, const protocol_wire::ContractInvitation &);
        void contractPrepared(const ConnectionIdType &, const Coin::typesafeOutPoint &);
        void pieceRequested(const ConnectionIdType & id, int i);
        void invalidPieceRequested(const ConnectionIdType & id);
        void paymentInterrupted(const ConnectionIdType & id);
        void receivedValidPayment(const ConnectionIdType & id, const Coin::Signature &);
        void receivedInvalidPayment(const ConnectionIdType & id, const Coin::Signature &);

    private:

        // Reference to core of session
        detail::SessionCoreImpl<ConnectionIdType> * _sessionCore;

        // State


        // Terms for selling
        protocol_wire::SellerTerms _terms;

        // When session was started
        time_t _lastStart;

    };
}
}

#endif // JOYSTREAM_PROTOCOL_SELLING_HPP
