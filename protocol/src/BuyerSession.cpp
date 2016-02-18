/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/BuyerSession.hpp>
#include <protocol/Utilities.hpp>
#include <cassert>

namespace joystream {
namespace protocol {

    BuyerSession::BuyerSession(Coin::Network network,
                               const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                               const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                               const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                               const std::map<std::string, BuyerConnection> & connections,
                               BuyerSessionState state,
                               const BuyerTerms & terms,
                               const joystream::paymentchannel::Payor & payor,
                               const std::vector<Seller> & sellers,
                               const std::vector<Piece> & pieces,
                               uint32_t assignmentLowerBound)
        : Session(Mode::buy, network, removedConnectionCallbackHandler, generateKeyPairsCallbackHandler, generateP2PKHAddressesCallbackHandler)
        , _connections(connections)
        , _state(state)
        , _terms(terms)
        , _payor(payor)
        , _sellers(sellers)
        , _pieces(pieces)
        , _assignmentLowerBound(assignmentLowerBound) {
    }

    BuyerSession * BuyerSession::createFreshSession(Coin::Network network,
                                                  const RemovedConnectionCallbackHandler & removedConnectionCallbackHandler,
                                                  const GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                                                  const GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                                                  const BuyerTerms & terms,
                                                  const std::vector<Piece> & pieces) {

        const std::vector<Seller> & sellers;

        return new BuyerSession(network,
                                removedConnectionCallbackHandler,
                                generateKeyPairsCallbackHandler,
                                generateP2PKHAddressesCallbackHandler,
                                std::map<std::string, BuyerConnection>(),
                                BuyerSessionState::waiting_for_full_set_of_sellers,
                                terms,
                                joystream::protocol::utilities::createPayorForNewBuyer(const Coin::UnspentP2PKHOutput & utxo,
                                                                                       Coin::KeyPair changeOutputKeyPair,
                                                                                       quint64 changeValue,
                                                                                       quint32 numberOfSellers),
                                sellers,
                                pieces,
                                0);
    }

    void BuyerSession::addConnection(const Connection & connection) {

        // Make sure connection is not already in session
        if(_connections.find(connection.peerName()) == _connections.cend())
            throw std::runtime_error("Connection already exists in session");

        // Create a (buyer) connection which is fresh, i.e. has never had any message transmitted
        BuyerConnection buyerConnection = BuyerConnection::createFreshConnection(connection);

        // Send sell mode message
        wire::Buy m(_terms);
        buyerConnection.sendMessageCallbackHandler()(&m);

        // Update state
        buyerConnection.setClientState(BuyerClientState::buyer_mode_announced);

        // Store in mapping
        _connections[buyerConnection.peerName()] = buyerConnection;
    }

    void BuyerSession::removeConnection(const std::string & name) {

    }

    void BuyerSession::processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message) {

    }

    void BuyerSession::tick() {

        if(_state == BuyerSessionState::waiting_for_full_set_of_sellers) {

            // for all invited to join contract: if idle, then ignore them??

            // if enough time as passed for us to try to
            // pick how many, and which sellers to choose, then
            // try to do that if we have enough who have elected to
            // join contract

            // if enough time has pased, then invite everyone
            // with good enough terms to join the contract
            // by signing refund.
            /**
            if(enough time passed since X?) {

                //
            }
            */

        } // else if()


    }

    uint32_t BuyerSession::assignmentLowerBound() const {
        return _assignmentLowerBound;
    }

    void BuyerSession::setAssignmentLowerBound(uint32_t assignmentLowerBound) {
        _assignmentLowerBound = assignmentLowerBound;
    }

    /**
    std::list<std::string> BuyerSession::getInvitedSellersIdling() const {



    }


    int BuyerSession::inviteSellers() {

        // Check that we are in correct state
        if(_state != BuyerSessionState::waiting_for_full_set_of_sellers)
            throw std::runtime_error("Invalid state, must be waiting_for_full_set_of_sellers.");

        // Find channels unassigned to any peer
        std::list<uint32_t> indexOfSellersToInvite;

        for(uint32_t i = 0;i < _sellers.size();i++) {

            // Get state of seller
            Seller::State stateOfSeller = _sellers[i].state();

            if(stateOfSeller == Seller::State::unassigned)
                indexOfSellersToInvite.push_back(i);
            else if(stateOfSeller == Seller::State::invited &&
                    _) // idle for long enough, throw out.

        }



        // X = contains channels (indexes) which needs to have peer assigned
        // Iterate slots
            // *if peer assigned, but not yet joined, check if timed out, if so, then mark
            // peer as not interested in given terms, put put slot in container X
            // *if not invited, then store that slot nr in some container X


        // Iterate peers:
            // if peer is in correct mode, and terms are better than cutoff, then
            // put in set Y

        // sort Y in terms terms of peers? (how to do this)

        // pick top size(X) peers from Y, and assign to slots in X.

            // terms are better than cut off terms

        // sort X using the comparison operator
    }
    */
    /**
    quint64 Payor::minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB, quint64 paychanSettlementFee) {
        return paychanSettlementFee*numberOfSellers + (maxPrice*numberOfSellers)*numberOfPiecesInTorrent + computeContractFee(numberOfSellers, feePerkB);
    }
    */
}
}
