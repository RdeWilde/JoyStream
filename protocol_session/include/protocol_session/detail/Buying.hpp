/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_DETAIL_BUYING_HPP
#define JOYSTREAM_PROTOCOLSESSION_DETAIL_BUYING_HPP

#include <protocol_session/BuyingState.hpp>
#include <protocol_session/detail/Piece.hpp>
#include <protocol_session/detail/Seller.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <CoinCore/CoinNodeData.h>

#include <vector>

namespace joystream {
namespace protocol_wire {
    class PieceData;
}
namespace protocol_statemachine {
    class AnnouncedModeAndTerms;
}
namespace protocol_session {
namespace detail {

    template <class ConnectionIdType>
    struct SessionCoreImpl;

    template <class ConnectionIdType>
    class Buying {

    public:

        class Policy {

        public:

            Policy(double minTimeBeforeBuildingContract)
                : _minTimeBeforeBuildingContract(minTimeBeforeBuildingContract) {
            }

            bool okToBuildContract(const time_t started) const {

                // Get current time
                time_t now = time(0);

                // Return whether sufficient time has passed since start
                return difftime(now, started) >= _minTimeBeforeBuildingContract;
            }

        private:

            // 1
            // The minimum amount of time (s) required before
            // trying to buil a contract
            double _minTimeBeforeBuildingContract;

            // 2
            // time to wait, at very least,

            // 3
            // How long to wait before trying to ask someone else for the same piece
            // if a peer is not responding, or responding too slowly

            // 4
            // Ranking peers for invite: invite random peers, or minimize on price

            // 5
            // Ranking peers for downloading

            // 6

        };

        Buying(detail::SessionCoreImpl<ConnectionIdType> *, const Coin::UnspentP2PKHOutput &, const Policy &);

        // Time out processing hook
        // NB: Later give some indication of how to set timescale for this call
        void tick();

        //// Client events

        // Connection with givne id has been added (ex-post)
        void connectionAdded(const ConnectionIdType &);

        // Connection with given id has been removed (ex-post)
        void connectionRemoved(const ConnectionIdType &);

        // Change to given state
        void to(BuyingState::State);

        // Update terms
        void updateTerms(const protocol_wire::BuyerTerms &);

        //// Handling callbacks from connections

        void peerAnnouncedModeAndTerms(const ConnectionIdType &, const protocol_statemachine::AnnouncedModeAndTerms &);
        void sellerHasJoined(const ConnectionIdType &);
        void sellerHasInterruptedContract(const ConnectionIdType &);
        void receivedFullPiece(const ConnectionIdType &, const protocol_wire::PieceData &);

    private:

        // Tries start downloading if possible
        bool tryToStartDownloading();








        //int64_t contractFee(selectedSellers) const;

        std::vector<int64_t> distributeFunds(int64_t funding, int64_t txFee, const std::vector<protocol_wire::SellerTerms> & selectedTerms, unsigned int numberOfPieces);
        void buildContract(); // ? const std::vector<ConnectionIdType> &

        //
        int tryToAssignAndRequestPiece(Seller<ConnectionIdType> & s);
        int getNextUnassignedPiece() const;

        // Reference to core of session
        detail::SessionCoreImpl<ConnectionIdType> * _sessionCore;

        // Funding for buyer
        Coin::UnspentP2PKHOutput _funding;

        // Controls behaviour of session
        Policy _policy;

        // State during buy mode
        BuyingState _state;

        // Terms for buying
        protocol_wire::BuyerTerms _terms;

        //
        std::map<ConnectionIdType, Seller<ConnectionIdType>> _sellers;

        // Contract transaction id
        // NB** Must be stored, as signatures are non-deterministic
        // contributions to the TxId, and hence discarding them
        // When segwit is enforced, this will no longer be neccessary.
        //Coin::TransactionId _contractTxId;
        Coin::Transaction _contractTx;

        //////////////////////////
        //// Piece management ////
        //////////////////////////

        // Pieces in torrent file: Should really be array?
        std::vector<Piece<ConnectionIdType>> _pieces;

        // Index of piece which was last assigned, is initially set to .
        // When downloading in order, one tries to find next unavailable piece
        // with index no less than this.
        uint32_t _lastAssignedPiece;

        // Index before which we should never assign a piece unless all pieces
        // with a greater index have been downloaded.
        // Is typically used when client desires to download media from som
        // mid way starting point.
        uint32_t _assignmentLowerBound;

        ///////////////////////////////////////////
        /// State below is dervied from _pieces ///
        ///////////////////////////////////////////

        // The number of pieces which have not been downloaded and not been assigned to a connection
        //uint32_t _numberOfUnassignedPieces;

        // Keeps track of lower bound for piece indexes which may be assigned.
        // Is updated when full pieces are downloaded contigously, and
        // is used with getNextUnassignedPiece() to find next piece to assign.
        //

        // When session was started
        time_t _lastStart;
        time_t _lastStartOfSendingInvitations;

    };

}
}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol_session/../../src/detail/Buying.cpp>

#endif // JOYSTREAM_PROTOCOLSESSION_DETAIL_BUYING_HPP
