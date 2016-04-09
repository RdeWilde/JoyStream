/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SESSION_HPP
#define JOYSTREAM_PROTOCOL_SESSION_HPP

#include <protocol/SessionMode.hpp>
#include <protocol/Connection.hpp>
#include <protocol/BuyerSessionState.hpp>
#include <protocol/Piece.hpp>
#include <protocol/Seller.hpp>
#include <wire/BuyerTerms.hpp>

namespace joystream {
namespace wire {
    class ExtendedMessagePayload;
}
namespace protocol {

    // NOTE:
    // ConnectionIdType: type for identifying connections, must
    // be possible to use as key in std::map, and also have
    // std::string ConnectionIdType::toString() const

    //// Callback types

    // Callback for handling the removal of a connection from the session: c++11 alias declaration
    template <class ConnectionIdType>
    using RemovedConnectionCallbackHandler = std::function<void(const ConnectionIdType &)>;

    typedef std::function<std::vector<Coin::KeyPair>(int)> GenerateKeyPairsCallbackHandler;
    typedef std::function<std::vector<Coin::P2PKHAddress>(int)> GenerateP2PKHAddressesCallbackHandler;

    // Callback for handling broadcasting a transaction
    //typedef std::function<bool(const Coin::Transaction &)> BroadCastTransactionCallbackHandler;

    // Callback for generating a key pair
    //typedef std::function< generate coin::keypair?

    // Callback for generating a receive address
    //typedef std::function generate address

    //// Session core implementation

    namespace detail {

        template <class ConnectionIdType>
        struct SessionCoreImpl {

            SessionCoreImpl(RemovedConnectionCallbackHandler<ConnectionIdType> removedConnectionCallbackHandler,
                            GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler,
                            GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler)
                : _mode(SessionMode::NotSet)
                , _removedConnectionCallbackHandler(removedConnectionCallbackHandler)
                , _generateKeyPairsCallbackHandler(generateKeyPairsCallbackHandler)
                , _generateP2PKHAddressesCallbackHandler(generateP2PKHAddressesCallbackHandler) {

                // Note starting time
                time(&_started);
            }

            bool hasConnection(const ConnectionIdType & id) const {
                return _connections.find(id) != _connections.cend();
            }

            // Mode of session
            SessionMode _mode;

            // Connections
            std::map<ConnectionIdType, Connection<ConnectionIdType> *> _connections;

            // Callback for when connection has been removed from session
            RemovedConnectionCallbackHandler<ConnectionIdType> _removedConnectionCallbackHandler;

            // Callback for when key pairs have to be generated
            GenerateKeyPairsCallbackHandler _generateKeyPairsCallbackHandler;

            // Callback for when addresses have to be generated
            GenerateP2PKHAddressesCallbackHandler _generateP2PKHAddressesCallbackHandler;

            // When session was started
            time_t _started;
        };

    }

    template <class ConnectionIdType>
    class Session {

    public:

        // Construct session
        Session(const RemovedConnectionCallbackHandler<ConnectionIdType> &,
                const GenerateKeyPairsCallbackHandler &,
                const GenerateP2PKHAddressesCallbackHandler &);

        //// Manage mode

        void toObserve();
        void toSellMode();
        void toBuyMode();

        //// Manage connections

        // Adds connection, and return the current number of connections
        uint addConnection(const Connection<ConnectionIdType> &);

        // Whether there is a connection with given id
        bool hasConnection(const ConnectionIdType &) const;

        // Remove connection if one exists with given id, otherwise returns false.
        // NB:does not result in correspondnig callback ??!?!
        bool removeConnection(const ConnectionIdType &);

        // Either we paid for it, or it just came in.
        //void markPieceAsDownloadedAndValid(int index);

        void processMessageOnConnection(const ConnectionIdType &, const wire::ExtendedMessagePayload *);

        ////

        //static quint64 minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB, quint64 paychanSettlementFee);


        SessionMode mode() const;

    private:

        // Connections can access private routines for handling state machine callbacks
        friend class Connection<ConnectionIdType>;

        //// Handling callbacks from connections
        void invitedToOutdatedContract(const ConnectionIdType &);
        void invitedToJoinContract(const ConnectionIdType &, const joystream::wire::ContractInvitation &);
        void contractPrepared(const ConnectionIdType &, const Coin::typesafeOutPoint &);
        void pieceRequested(const ConnectionIdType & id, int i);
        void invalidPieceRequested(const ConnectionIdType & id);
        void paymentInterrupted(const ConnectionIdType & id);
        void receivedValidPayment(const ConnectionIdType & id, const Coin::Signature &);
        void receivedInvalidPayment(const ConnectionIdType & id, const Coin::Signature &);
        void sellerHasJoined(const ConnectionIdType & id);
        void sellerHasInterruptedContract(const ConnectionIdType & id);
        void receivedFullPiece(const ConnectionIdType & id, const joystream::wire::PieceData &);

        uint32_t determineNumberOfSellers() const;
        void setNumberOfSellers(uint32_t n);

        // ...
        int inviteSellers();

        // Session core
        detail::SessionCoreImpl<ConnectionIdType> _core;

        //// Observer

        //// Seller

        //// Buyer

        // State during buy mode
        BuyerSessionState _state;

        // Terms for buying
        joystream::wire::BuyerTerms _terms;

        // Sellers: Should be really be array?
        std::vector<Seller> _sellers;

        // Pieces in torrent file: Should really be array?
        std::vector<Piece> _pieces;

        // Is required to ensure in order downloading from correct position in file
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

    };

}
}

// Needed due to c++ needing implementation for all uses of templated types
#include <protocol/../../src/Session.cpp>

#endif // JOYSTREAM_PROTOCOL_SESSION_HPP

