/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SESSION_BUY_SESSION_HPP
#define JOYSTREAM_PROTOCOL_SESSION_BUY_SESSION_HPP

#include <protocol/Session.hpp>
#include <protocol/buy/Connection.hpp>
#include <protocol/buy/SessionState.hpp>
#include <protocol/buy/Terms.hpp>
#include <protocol/buy/Piece.hpp>
#include <paymentchannel/Payor.hpp>

namespace joystream {
namespace protocol {

namespace sell {
    class Terms;
    class Session;
}

namespace observer {
    class Session;
}

namespace buy {

    class Session : public joystream::protocol::Session {

    public:

        Session(const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                std::map<std::string, Connection> connections,
                SessionState state,
                const Terms & terms,
                const joystream::paymentchannel::Payor & payor,
                const std::vector<Piece> & pieces);

        // Update terms in the same mode
        void updateTerms(const Terms & terms);

        // Returns session for corresponding new mode, after sending appropriate messages to all active peers,
        // and returned object is owned by callee.
        joystream::protocol::sell::Session * switchToSellMode(const joystream::protocol::sell::Terms & terms);
        joystream::protocol::observer::Session * switchToObserveMode();

        // Either we paid for it, or it just came in.
        //void markPieceAsDownloadedAndValid(int index);

        // Add connection with peer before any communicaiton, beyond plain bittorrent handshake has started (not even
        virtual void addConnection(const Connection & connection);

        // Remove connection: does not result in correspondnig callback
        virtual void removeConnection(const std::string & name);

        // Process extended message: does not take ownership of message
        virtual void processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload * message);


    private:

        // Mapping peer name to corresponding connection with peer
        std::map<std::string, Connection> _connections;

        // State during buy mode
        SessionState _state;

        // Terms for buying
        Terms _terms;

        // Payor side of payment channel
        joystream::paymentchannel::Payor _payor;

        ////////////////////////
        /// Piece management ///
        ////////////////////////

        // Pieces in torrent file
        std::vector<Piece> _pieces;

        // State below is dervied from _pieces

        // The number of pieces which have not been downloaded and not been assigned to a connection
        uint32_t _numberOfUnassignedPieces;
        //
        // Set of names for connections which have not been assigned a piece.
        //
        // the tick() callback routinely attempts to assign a piece
        // to a peer plugin in this set.
        std::queue<std::string> _namesOfConnectionsWithoutPieceAssignment;
        //
        // Keeps track of lower bound for piece indexes which may be assigned.
        // Is updated when full pieces are downloaded contigously, and
        // is used with getNextUnassignedPiece() to find next piece to assign.
        //
        // Is required to ensure in order downloading from correct position in file, e.g.,
        // if user has seeked to this position recently.
        uint32_t _assignmentLowerBound;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_SESSION_BUY_SESSION_HPP

