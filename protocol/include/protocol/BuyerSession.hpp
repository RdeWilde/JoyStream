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

    class BuyerSession : public Session<BuyerConnection> {

    public:

        // Construct fully specified session
        BuyerSession(Coin::Network network,
                     const std::map<std::string, BuyerConnection> & connections,
                     const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                     const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                     const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                     BuyerSessionState state,
                     const BuyerTerms & terms,
                     const joystream::paymentchannel::Payor & payor,
                     const std::vector<Seller> & sellers,
                     const std::vector<Piece> & pieces,
                     uint32_t _assignmentLowerBound);

        // Construct session without any prior state
        static BuyerSession * createFreshSession(Coin::Network network,
                                                 const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                                 const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                                 const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                                 const BuyerTerms & terms,
                                                 const Coin::UnspentP2PKHOutput & utxo,
                                                 const Coin::P2PKHAddress & changeAddress,
                                                 const std::vector<Piece> & pieces);

        // Construct session based on preexisting session
        template <class T>
        static BuyerSession * convertToBuyerSession(const Session<T> * session, const BuyerTerms & terms, const Coin::UnspentP2PKHOutput & utxo, const std::vector<Piece> & pieces);

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

        virtual bool addFreshConnection(const Connection & connection);
        virtual bool removeConnection(const std::string & name);
        virtual void processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message);
        virtual void tick();

        //static quint64 minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB, quint64 paychanSettlementFee);

        uint32_t assignmentLowerBound() const;
        void setAssignmentLowerBound(uint32_t assignmentLowerBound);

    private:

        uint32_t determineNumberOfSellers() const;
        void setNumberOfSellers(uint32_t n);

        // ...
        int inviteSellers();

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

        // Is required to ensure in order downloading from correct position in file
        uint32_t _assignmentLowerBound;

        // When session was started
        time_t _sessionStarted;

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

    template <class T>
    BuyerSession * BuyerSession::convertToBuyerSession(const Session<T> * session, const BuyerTerms & terms, const Coin::UnspentP2PKHOutput & utxo, const std::vector<Piece> & pieces) {

        // Get callback for generating addresses
        typename Session<T>::GenerateP2PKHAddressesCallbackHandler handler = session->generateP2PKHAddressesCallbackHandler();

        // Create (buyer) session
        BuyerSession * buyerSession = BuyerSession::createFreshSession(session->network(),
                                                                       session->removedConnectionCallbackHandler(),
                                                                       session->generateKeyPairsCallbackHandler(),
                                                                       handler,
                                                                       terms,
                                                                       utxo,
                                                                       handler(1).front(),
                                                                       pieces);

        // Get connections
        std::map<std::string, T> connections = session->connections();

        // Add all connections to session
        for(typename std::map<std::string, T>::const_iterator i = connections.cbegin(); i != connections.cend();i++)
            buyerSession->addFreshConnection((*i).second);

        return buyerSession;
    }

}
}

#endif // JOYSTREAM_PROTOCOL_SESSION_BUYER_SESSION_HPP

