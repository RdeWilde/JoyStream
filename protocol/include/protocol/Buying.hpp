/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUYING_HPP
#define JOYSTREAM_PROTOCOL_BUYING_HPP

#include <protocol/BuyerSessionState.hpp>
#include <protocol/Piece.hpp>
#include <protocol/Seller.hpp>
#include <wire/BuyerTerms.hpp>

#include <vector>

namespace joystream {
namespace wire {
    class PieceData;
}
namespace protocol {

    namespace detail {

        template <class ConnectionIdType>
        struct SessionCoreImpl;
    }

    template <class ConnectionIdType>
    class Buying {

    public:

        Buying(detail::SessionCoreImpl<ConnectionIdType> *);

        //// Handling callbacks from connections

        void sellerHasJoined(const ConnectionIdType &);
        void sellerHasInterruptedContract(const ConnectionIdType &);
        void receivedFullPiece(const ConnectionIdType &, const joystream::wire::PieceData &);

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
        joystream::wire::BuyerTerms _terms;

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
