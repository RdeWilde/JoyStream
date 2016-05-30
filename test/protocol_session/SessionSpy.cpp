/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, May 9 2016
 */

#include "SessionSpy.hpp"

template <class ConnectionIdType>
SessionSpy<ConnectionIdType>::SessionSpy(const GenerateKeyPairsCallbackHandler & kpHandler,
                                         const GenerateP2PKHAddressesCallbackHandler & p2pkhHandler,
                                         const BroadcastTransaction & broadcast,
                                         Session<ConnectionIdType> * session)
    : generateKeyPairsCallbackSlot(kpHandler)
    , generateP2PKHAddressesCallbackSlot(p2pkhHandler)
    , broadcastTransactionCallbackSlot(broadcast)
    , _session(session) {
}

template <class ConnectionIdType>
SessionSpy<ConnectionIdType>::~SessionSpy() {
    removeConnectionSpies();
}

template <class ConnectionIdType>
void SessionSpy<ConnectionIdType>::toMonitoredObserveMode() {
    _session->toObserveMode(removedConnectionCallbackSlot.hook());
}

template <class ConnectionIdType>
void SessionSpy<ConnectionIdType>::toMonitoredSellMode(const SellingPolicy & policy,
                                                       const protocol_wire::SellerTerms & terms,
                                                       int maxPiecePrice) {

    _session->toSellMode(removedConnectionCallbackSlot.hook(),
                         generateKeyPairsCallbackSlot.hook(),
                         generateP2PKHAddressesCallbackSlot.hook(),
                         loadPieceForBuyerCallbackSlot.hook(),
                         claimLastPaymentCallbackSlot.hook(),
                         anchorAnnouncedCallbackSlot.hook(),
                         policy,
                         terms,
                         maxPiecePrice);

}

template <class ConnectionIdType>
void SessionSpy<ConnectionIdType>::toMonitoredBuyMode(const Coin::UnspentP2PKHOutput & funding,
                                                      const BuyingPolicy & policy,
                                                      const protocol_wire::BuyerTerms & terms,
                                                      const TorrentPieceInformation & information) {
    _session->toBuyMode(removedConnectionCallbackSlot.hook(),
                         generateKeyPairsCallbackSlot.hook(),
                         generateP2PKHAddressesCallbackSlot.hook(),
                         broadcastTransactionCallbackSlot.hook(),
                         fullPieceArrivedCallbackSlot.hook(),
                         funding,
                         policy,
                         terms,
                         information);
}

template <class ConnectionIdType>
ConnectionSpy<ConnectionIdType> * SessionSpy<ConnectionIdType>::addConnection(const ConnectionIdType & id) {

    // Create spy for connection
    ConnectionSpy<ConnectionIdType> * spy = new ConnectionSpy<ConnectionIdType>(id);

    // Create connection, spied on by spy, and add to underlying session
    // NB*: may cause exception if connecton id already has been added, in which case spy leaks
    _session->addConnection(id, spy->sendMessageOnConnectionCallbackSlot.hook());

    // Insert spy into mapping
    connectionSpies.insert(std::make_pair(id, spy));

    return spy;
}

template <class ConnectionIdType>
void SessionSpy<ConnectionIdType>::removeConnectionSpies() {

    for(auto mapping : connectionSpies)
        delete mapping.second;

    connectionSpies.clear();
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::blank() const {

    for(auto mapping : connectionSpies) {
        if(!mapping.second->blank())
            return false;
    }

    return blankSession();
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::blankSession() const {

            //// General
    return  removedConnectionCallbackSlot.empty() &&
            generateKeyPairsCallbackSlot.empty() &&
            generateP2PKHAddressesCallbackSlot.empty() &&
            //// Buying
            broadcastTransactionCallbackSlot.empty() &&
            fullPieceArrivedCallbackSlot.empty() &&
            //// Selling
            loadPieceForBuyerCallbackSlot.empty() &&
            claimLastPaymentCallbackSlot.empty() &&
            anchorAnnouncedCallbackSlot.empty();
}


template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::blankConnectionExcept(const ConnectionIdType & id) const {

    for(auto mapping : connectionSpies) {

        // any connection, different from id, which is not blank, leads to false result
        if(mapping.first != id && !mapping.second->blank())
            return false;
    }

    return true;
}

template <class ConnectionIdType>
void SessionSpy<ConnectionIdType>::reset() {

    //// General
    removedConnectionCallbackSlot.clear();
    generateKeyPairsCallbackSlot.clear();
    generateP2PKHAddressesCallbackSlot.clear();

    //// Buying
    broadcastTransactionCallbackSlot.clear();
    fullPieceArrivedCallbackSlot.clear();

    //// Selling
    loadPieceForBuyerCallbackSlot.clear();
    claimLastPaymentCallbackSlot.clear();
    anchorAnnouncedCallbackSlot.clear();

    // Connection spies
    for(auto mapping : connectionSpies)
        mapping.second->reset();
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::onlyCalledRemovedConnection() const {
    return  //// General
            !removedConnectionCallbackSlot.empty() &&
            generateKeyPairsCallbackSlot.empty() &&
            generateP2PKHAddressesCallbackSlot.empty() &&
            //// Buying
            broadcastTransactionCallbackSlot.empty() &&
            fullPieceArrivedCallbackSlot.empty() &&
            //// Selling
            loadPieceForBuyerCallbackSlot.empty() &&
            claimLastPaymentCallbackSlot.empty() &&
            anchorAnnouncedCallbackSlot.empty();
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::onlyCalledGenerateKeyPairs() const {
    return  //// General
            removedConnectionCallbackSlot.empty() &&
            !generateKeyPairsCallbackSlot.empty() &&
            generateP2PKHAddressesCallbackSlot.empty() &&
            //// Buying
            broadcastTransactionCallbackSlot.empty() &&
            fullPieceArrivedCallbackSlot.empty() &&
            //// Selling
            loadPieceForBuyerCallbackSlot.empty() &&
            claimLastPaymentCallbackSlot.empty() &&
            anchorAnnouncedCallbackSlot.empty();
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::onlyCalledGenerateP2PKHAddresses() const {
    return  //// General
            removedConnectionCallbackSlot.empty() &&
            generateKeyPairsCallbackSlot.empty() &&
            !generateP2PKHAddressesCallbackSlot.empty() &&
            //// Buying
            broadcastTransactionCallbackSlot.empty() &&
            fullPieceArrivedCallbackSlot.empty() &&
            //// Selling
            loadPieceForBuyerCallbackSlot.empty() &&
            claimLastPaymentCallbackSlot.empty() &&
            anchorAnnouncedCallbackSlot.empty();
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::onlyCalledBroadcastTransaction() const {
    return  //// General
            removedConnectionCallbackSlot.empty() &&
            generateKeyPairsCallbackSlot.empty() &&
            generateP2PKHAddressesCallbackSlot.empty() &&
            //// Buying
            !broadcastTransactionCallbackSlot.empty() &&
            fullPieceArrivedCallbackSlot.empty() &&
            //// Selling
            loadPieceForBuyerCallbackSlot.empty() &&
            claimLastPaymentCallbackSlot.empty() &&
            anchorAnnouncedCallbackSlot.empty();
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::onlyCalledFullPieceArrived() const {
    return  //// General
            removedConnectionCallbackSlot.empty() &&
            generateKeyPairsCallbackSlot.empty() &&
            generateP2PKHAddressesCallbackSlot.empty() &&
            //// Buying
            broadcastTransactionCallbackSlot.empty() &&
            !fullPieceArrivedCallbackSlot.empty() &&
            //// Selling
            loadPieceForBuyerCallbackSlot.empty() &&
            claimLastPaymentCallbackSlot.empty() &&
            anchorAnnouncedCallbackSlot.empty();
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::onlyCalledLoadPieceForBuyer() const {
    return  //// General
            removedConnectionCallbackSlot.empty() &&
            generateKeyPairsCallbackSlot.empty() &&
            generateP2PKHAddressesCallbackSlot.empty() &&
            //// Buying
            broadcastTransactionCallbackSlot.empty() &&
            fullPieceArrivedCallbackSlot.empty() &&
            //// Selling
            !loadPieceForBuyerCallbackSlot.empty() &&
            claimLastPaymentCallbackSlot.empty() &&
            anchorAnnouncedCallbackSlot.empty();
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::onlyCalledClaimLastPayment() const {
    return  //// General
            removedConnectionCallbackSlot.empty() &&
            generateKeyPairsCallbackSlot.empty() &&
            generateP2PKHAddressesCallbackSlot.empty() &&
            //// Buying
            broadcastTransactionCallbackSlot.empty() &&
            fullPieceArrivedCallbackSlot.empty() &&
            //// Selling
            loadPieceForBuyerCallbackSlot.empty() &&
            !claimLastPaymentCallbackSlot.empty() &&
            anchorAnnouncedCallbackSlot.empty();
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::onlyCalledAnchorAnnounced() const {
    return  //// General
            removedConnectionCallbackSlot.empty() &&
            generateKeyPairsCallbackSlot.empty() &&
            generateP2PKHAddressesCallbackSlot.empty() &&
            //// Buying
            broadcastTransactionCallbackSlot.empty() &&
            fullPieceArrivedCallbackSlot.empty() &&
            //// Selling
            loadPieceForBuyerCallbackSlot.empty() &&
            claimLastPaymentCallbackSlot.empty() &&
            !anchorAnnouncedCallbackSlot.empty();
}
