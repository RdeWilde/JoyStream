/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUYING_HPP
#define JOYSTREAM_PROTOCOL_BUYING_HPP

#include <protocol_session/BuyerSessionState.hpp>
#include <protocol_session/Piece.hpp>
#include <protocol_session/Seller.hpp>
#include <protocol_wire/BuyerTerms.hpp>

#include <vector>

namespace joystream {
namespace protocol_wire {
    class PieceData;
}
namespace  protocol_statemachine {
    class AnnouncedModeAndTerms;
}
namespace protocol_session {

    namespace detail {

        template <class ConnectionIdType>
        struct SessionCoreImpl;
    }

    template <class ConnectionIdType>
    class Buying {

    public:

        class Policy {

        public:

        private:

            // time to wait, at very least,

            // Ranking peers for invite: invite random peers, or minimize on price

            // Ranking peers for downloading
        };

        Buying(detail::SessionCoreImpl<ConnectionIdType> *);

        // Time out processing hook
        // NB: Later give some indication of how to set timescale for this call
        void tick();

        //// Connection events (ex-post)

        void connectionAdded(const ConnectionIdType &);
        void connectionRemoved(const ConnectionIdType &);

        //// Handling callbacks from connections

        void peerAnnouncedModeAndTerms(const ConnectionIdType &, const protocol_statemachine::AnnouncedModeAndTerms &);
        void sellerHasJoined(const ConnectionIdType &);
        void sellerHasInterruptedContract(const ConnectionIdType &);
        void receivedFullPiece(const ConnectionIdType &, const protocol_wire::PieceData &);

    private:

        uint32_t determineNumberOfSellers() const;
        void setNumberOfSellers(uint32_t n);

        // ...
        int inviteSellers();

        // Reference to core of session
        detail::SessionCoreImpl<ConnectionIdType> * _sessionCore;

        // State during buy mode
        BuyerSessionState _state;

        // Terms for buying
        protocol_wire::BuyerTerms _terms;

        // Sellers: Should be really be array?
        std::vector<Seller<ConnectionIdType>> _sellers;

        // Pieces in torrent file: Should really be array?
        std::vector<Piece> _pieces;

        // Is required to ensure in order downloading from correct position in file
        uint32_t _assignmentLowerBound;

        // When session was started
        time_t _lastStart;

        ///////////////////////////////////////////
        /// State below is dervied from _pieces ///
        ///////////////////////////////////////////

        // The number of pieces which have not been downloaded and not been assigned to a connection
        //uint32_t _numberOfUnassignedPieces;

        // Keeps track of lower bound for piece indexes which may be assigned.
        // Is updated when full pieces are downloaded contigously, and
        // is used with getNextUnassignedPiece() to find next piece to assign.
        //

    };

}
}

#endif // JOYSTREAM_PROTOCOL_BUYING_HPP
