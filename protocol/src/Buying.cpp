/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 9 2016
 */

#include <protocol/Buying.hpp>
#include <wire/PieceData.hpp>

namespace joystream {
namespace protocol {

    template <class ConnectionIdType>
    Buying<ConnectionIdType>::Buying(detail::SessionCoreImpl<ConnectionIdType> * sessionCore)
        : _sessionCore(sessionCore){

        // Note starting time
        time(&_lastStart);
    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::sellerHasJoined(const ConnectionIdType & id) {

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::sellerHasInterruptedContract(const ConnectionIdType & id) {

    }

    template <class ConnectionIdType>
    void Buying<ConnectionIdType>::receivedFullPiece(const ConnectionIdType & id, const joystream::wire::PieceData & p) {

    }

    /*
    bool Buyer::addFreshConnection(const Connection & connection) {

        // Make sure connection is not already in session
        if(hasConnection(connection.peerName()))
            return false;

        // Create a (buyer) connection which is fresh, i.e. has never had any message transmitted
        BuyerConnection buyerConnection = BuyerConnection::createFreshConnection(connection);

        // Send sell mode message
        joystream::wire::Buy m(_terms);
        buyerConnection.sendMessageCallbackHandler()(&m);

        // Update state
        buyerConnection.setClientState(BuyerClientState::buyer_mode_announced);

        // Store mapping
        _connections[buyerConnection.peerName()] = buyerConnection;

        return true;
    }

    bool Buyer::removeConnection(const std::string & name) {


    }

    void Buyer::processMessageOnConnection(const std::string & name, const wire::ExtendedMessagePayload & message) {

        if(!hasConnection(name))
            throw std::runtime_error("No connection with given id exists.");

        // Make sure new mode messages cannot be sent that change terms of contracts underway



    }

    void Buyer::tick() {

        if(_state == BuyerSessionState::waiting_to_decide_how_many_sellers_to_have) {

            // Get current time
            time_t now = time(0);

            // if enough time has passed, decide
            //if(_sessionStarted < )
                return;

            uint32_t numberOfSellers = determineNumberOfSellers();

            // Set number of sellers
            setNumberOfSellers(numberOfSellers);

        } else if(_state == BuyerSessionState::waiting_for_full_set_of_sellers_with_signed_refund) {

            // for all invited to join contract: if idle, then ignore them??

            // if enough time as passed for us to try to
            // pick how many, and which sellers to choose, then
            // try to do that if we have enough who have elected to
            // join contract

            // if enough time has pased, then invite everyone
            // with good enough terms to join the contract
            // by signing refund.

            //if(enough time passed since X?) {
            //
            //
            //}


        } // else if()
    }

    uint32_t Buyer::assignmentLowerBound() const {
        return _assignmentLowerBound;
    }

    void Buyer::setAssignmentLowerBound(uint32_t assignmentLowerBound) {
        _assignmentLowerBound = assignmentLowerBound;
    }

    uint32_t Buyer::determineNumberOfSellers() const {

    }

    void Buyer::setNumberOfSellers(uint32_t n) {

        for(uint32_t i = 0;i < n;i++) {

            //Seller s(Seller::State::unassigned)



        }

    }

    int BuyerSession::computeNumberOfSellers () {

        // min_funds per channel = max( dust limit + fee required to refund output, being able to pay fo full thing at my own price point with one channel)
        // use total utxo balance and changevalue to figure out if we have neough to cover min_funds*#channels

    }

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

    quint64 Payor::minimalFunds(quint32 numberOfPiecesInTorrent, quint64 maxPrice, int numberOfSellers, quint64 feePerkB, quint64 paychanSettlementFee) {
        return paychanSettlementFee*numberOfSellers + (maxPrice*numberOfSellers)*numberOfPiecesInTorrent + computeContractFee(numberOfSellers, feePerkB);
    }
    */

}
}
