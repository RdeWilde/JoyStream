/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 4 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SESSION_HPP
#define JOYSTREAM_PROTOCOL_SESSION_HPP

#include <protocol/BuyModeSessionState.hpp>
#include <protocol/Mode.hpp>
#include <protocol/Connection.hpp>
#include <paymentchannel/Payor.hpp>

#include <map>
#include <vector>

namespace joystream {
namespace protocol {

    class ExtendedMessagePayload;

    class Session {

    public:

        //static Session buyer(); // worst buy terms,
        //static Session seller(); // worst sale terms, read piece callback,
        //static Session observer();

        // Switch from one mode to another
        //void switchToBuyer();
        //void switchToSeller();
        //void switchToObserver();

        // Add connection
        void addConnection(const std::string & name, ); // write message hook for connection?

        // Remove connection
        void removeConnection(const std::string & name);

        // Process extended message: does not take ownership of message
        void processMessageOnConnection(const std::string & name, const ExtendedMessagePayload * message);

        /**
         * Actions
         */

        // Buyer

        // Seller

        // What about actions on a given connection??: remov, delete, error



    private:

        // Mode
        Mode _mode;

        // <INFORMATION ABOUT TORRENT>

        // Connections
        std::map<std::string, Connection > _connections;

        //////////////
        /// BUYER MODE
        //////////////

        // State during buy mode
        BuyModeSessionState _buyModeState;

        // Maximum price accepted (satoshies)
        int64_t _maxPrice;

        // Maximum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
        uint32_t _maxLock;

        // Maximum fee per byte in contract transaction (satoshies)
        int64_t _maxFeePerKb;

        // Number of sellers
        uint32_t _numberOfSellers;

        // Payor side of payment channel
        joystream::paymentchannel::Payor _payor;

        //
        // Buy mode: Piece management
        //

        // Pieces in torrent file
        //std::vector<Piece> _pieces;

        // The number of pieces which have not been downloaded and not been assigned to a connection
        uint32_t _numberOfUnassignedPieces;


        // Set of names for connections which have not been assigned a piece.

        //********
        // the tick() callback routinely attempts to assign a piece
        // to a peer plugin in this set.
        std::set<std::string> _namesOfConnectionsWithoutPieceAssignment;

        // Keeps track of lower bound for piece indexes which may be assigned.
        // Is updated when full pieces are downloaded contigously, and
        // is used with getNextUnassignedPiece() to find next piece to assign.
        //
        // Is required to ensure in order downloading from correct position in file, e.g.,
        // if user has seeked to this position recently.
        uint32_t _assignmentLowerBound;

        ///////////////
        /// SELLER MODE
        ///////////////

    };
}
}

#endif // JOYSTREAM_PROTOCOL_SESSION_HPPs
