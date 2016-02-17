/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SESSION_BUYER_SESSION_HPP
#define JOYSTREAM_PROTOCOL_SESSION_BUYER_SESSION_HPP

#include <protocol/Session.hpp>
#include <protocol/BuyerConnection.hpp>
#include <protocol/BuyerSessionState.hpp>
#include <protocol/BuyerTerms.hpp>
#include <protocol/Piece.hpp>
#include <protocol/Seller.hpp>
#include <paymentchannel/Payor.hpp>

namespace joystream {
namespace protocol {

    class SellerTerms;
    class SellerSession;
    class ObserverSession;
    class Connection;

    class BuyerSession : public Session {

    public:

        BuyerSession(Coin::Network network,
                     const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                     const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                     const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                     const std::map<std::string, BuyerConnection> & connections,
                     BuyerSessionState state,
                     const BuyerTerms & terms,
                     const joystream::paymentchannel::Payor & payor,
                     const std::vector<Seller> & sellers,
                     const std::vector<Piece> & pieces);

        /**
        // Update terms in the same mode
        void updateTerms(const Terms & terms);

        // Returns session for corresponding new mode, after sending appropriate messages to all active peers,
        // and returned object is owned by callee.
        joystream::protocol::sell::Session * switchToSellMode(const joystream::protocol::sell::Terms & terms);
        joystream::protocol::observer::Session * switchToObserveMode();
        */

        // Either we paid for it, or it just came in.
        //void markPieceAsDownloadedAndValid(int index);

        virtual void addConnection(const Connection & connection);
        virtual void removeConnection(const std::string & name);
        virtual void processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message);
        virtual void tick();

        //static quint64 minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB, quint64 paychanSettlementFee);

    private:

        // ...
        int inviteSellers();

        // Mapping peer name to corresponding connection with peer
        std::map<std::string, BuyerConnection> _connections;

        // State during buy mode
        BuyerSessionState _state;

        // Terms for buying
        BuyerTerms _terms;

        // Payor side of payment channel
        joystream::paymentchannel::Payor _payor;

        // Sellers
        std::vector<Seller> _sellers;

        // Pieces in torrent file
        std::vector<Piece> _pieces;

        ///////////////////////////////////////////
        /// State below is dervied from _pieces ///
        ///////////////////////////////////////////

        // The number of pieces which have not been downloaded and not been assigned to a connection
        uint32_t _numberOfUnassignedPieces;

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

#endif // JOYSTREAM_PROTOCOL_SESSION_BUYER_SESSION_HPP

