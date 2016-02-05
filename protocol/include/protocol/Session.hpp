/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SESSION_HPP
#define JOYSTREAM_PROTOCOL_SESSION_HPP

#include <protocol/Mode.hpp>
#include <protocol/Connection.hpp>
#include <protocol/buy/SessionState.hpp>
#include <protocol/buy/Terms.hpp>
#include <protocol/Piece.hpp>
#include <paymentchannel/Payor.hpp>

#include <functional>
#include <map>
#include <vector>

namespace joystream {
namespace protocol {

    namespace wire {
        class ExtendedMessagePayload;
    }

    class Session {

    public:

        // Callback for handling the removal of a connection from the session
        typedef std::function<void(const std::string &)> RemovedConnectionCallbackHandler;

        // Named constructors corresponding to mode
        static Session buyer(uint32_t numberOfPiecesInTorrent, const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler, buy::Terms terms);
        static Session seller(uint32_t numberOfPiecesInTorrent, const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler, sell::Terms terms);
        static Session observer(uint32_t numberOfPiecesInTorrent, const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler);

        // Add connection
        // ************** PROBLEM ********************
        // We need different args for different connection mode, spesifically the seller needs a read piece callback


        // solution 1: move callbacks to be session level (should we pass name, or do we do buffering internally)



        void addConnection(const std::string & name, const Connection::SendMessageCallbackHandler & connectionSendCallback);


        // Remove connection: does not result in correspondnig callback
        void removeConnection(const std::string & name);

        // Process extended message: does not take ownership of message
        void processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload * message);

        // How to control things like:
        // *start ----
        // *stop ----
        // *pause ----

        // Switch from one mode to another
        void switchToBuyMode(const buy::Terms & terms);
        void switchToSellMode(const sell::Terms & terms);
        void switchToObserveMode();

        // Update terms in the same mode
        void updateTerms(const buy::Terms & terms);
        void updateTerms(const sell::Terms & terms);

        //
        void markPieceAsDownloadedAndValid(int index);

        // Getters


    private:

        // Private constructor used by named constructors
        Session(Mode mode,
                uint32_t numberOfPiecesInTorrent,
                const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                const std::map<std::string, Connection> & connections,
                const buy::SessionState & buyModeState,
                const buy::Terms & buyTerms,
                const joystream::paymentchannel::Payor & payor,
                const std::vector<Piece> & pieces,
                const sell::Terms & sellTerms);

        // Mode
        Mode _mode;

        // The number of pieces in the torrent in question
        uint32_t _numberOfPiecesInTorrent;

        // Callback for when connection has been removed from session
        RemovedConnectionCallbackHandler _removedConnectionCallbackHandler;

        // Connections
        std::map<std::string, Connection> _connections;

        //////////////////
        /// BUYER MODE ///
        //////////////////

        // State during buy mode
        buy::SessionState _buyModeState;

        // Terms for buying
        buy::Terms _buyTerms;

        // Payor side of payment channel
        joystream::paymentchannel::Payor _payor;

        // *** Piece management

        // Pieces in torrent file
        std::vector<Piece> _pieces;

        /**
        // The number of pieces which have not been downloaded and not been assigned to a connection
        uint32_t _numberOfUnassignedPieces;
        //
        // Set of names for connections which have not been assigned a piece.
        //
        // the tick() callback routinely attempts to assign a piece
        // to a peer plugin in this set.
        std::set<std::string> _namesOfConnectionsWithoutPieceAssignment;
        //
        // Keeps track of lower bound for piece indexes which may be assigned.
        // Is updated when full pieces are downloaded contigously, and
        // is used with getNextUnassignedPiece() to find next piece to assign.
        //
        // Is required to ensure in order downloading from correct position in file, e.g.,
        // if user has seeked to this position recently.
        uint32_t _assignmentLowerBound;
        */

        ///////////////////
        /// SELLER MODE ///
        ///////////////////

        // Terms for selling, all new connections will have these terms,
        // all existing ones which are beyond mode state will keep their own
        // terms for rest session (until potential reset)
        sell::Terms _sellTerms;
    };
}
}

#endif // JOYSTREAM_PROTOCOL_SESSION_HPPs
