/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 29 2016
 */



#include <protocol_session/Session.hpp>
#include <protocol_session/detail/Observing.hpp>

namespace joystream {
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    Observing<ConnectionIdType>::Observing(Session<ConnectionIdType> * session)
        : _session(session) {
    }

    template <class ConnectionIdType>
    void Observing<ConnectionIdType>::tick() {

    }

    template<class ConnectionIdType>
    uint Observing<ConnectionIdType>::removeConnection(const ConnectionIdType & id) {

        assert(_session->hasConnection(id))
        assert(_session->state() != SessionState::stopped); // We cannot have connection and be stopped

        // Do we need to do anything here? we may be owed payment
    }

    template<class ConnectionIdType>
    void Observing<ConnectionIdType>::processMessageOnConnection(const ConnectionIdType &, const protocol_wire::ExtendedMessagePayload & m) {

        assert(_session->hasConnection(id));
        assert(_session->state() != SessionState::stopped); // We cannot have connection and be stopped

        // Get connection
        detail::Connection<ConnectionIdType> * c = _session->get(id);

        // and, have it process the message
        c->processMessage(m);
    }

    template<class ConnectionIdType>
    void Observing<ConnectionIdType>::pieceLoaded(const protocol_wire::PieceData &, int) {

    }

    template<class ConnectionIdType>
    void Observing<ConnectionIdType>::updateTerms(const protocol_wire::SellerTerms & terms) {

    }

    template<class ConnectionIdType>
    Selling<ConnectionIdType> * Observing<ConnectionIdType>::toBuyMode() {

    }

    template<class ConnectionIdType>
    void Observing<ConnectionIdType>::start() {

    }

    template<class ConnectionIdType>
    void Observing<ConnectionIdType>::stop() {

    }

    template<class ConnectionIdType>
    void Observing<ConnectionIdType>::pause() {

    }

}
}
}
