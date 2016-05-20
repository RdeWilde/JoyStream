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

    for(auto mapping : connectionSpies)
        delete mapping.second;
}

template <class ConnectionIdType>
void SessionSpy<ConnectionIdType>::toMonitoredObserveMode() {
    _session->toObserveMode();
}

template <class ConnectionIdType>
void SessionSpy<ConnectionIdType>::toMonitoredSellMode(const SellingPolicy & policy,
                                                       const protocol_wire::SellerTerms & terms) {

    _session->toSellMode(removedConnectionCallbackSlot.hook(),
                          generateKeyPairsCallbackSlot.hook(),
                          generateP2PKHAddressesCallbackSlot.hook(),
                          loadPieceForBuyerCallbackSlot.hook(),
                          claimLastPaymentCallbackSlot.hook(),
                          anchorAnnouncedCallbackSlot.hook(),
                          policy,
                          terms);

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

    if(connectionSpies.find(id) != connectionSpies.end())
        throw std::runtime_error("There is alreay a spy for this connection.");

    assert(!_session->hasConnection(id));

    // Create spy for connection
    ConnectionSpy<ConnectionIdType> * spy = new ConnectionSpy<ConnectionIdType>(id);

    // Create connection, spied on by spy, and add to underlying session
    _session->addConnection(id, spy->sendMessageOnConnectionCallbackSlot.hook());

    // Insert spy into mapping
    connectionSpies.insert(std::make_pair(id, spy));

    return spy;
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::noSessionEvents() const {

            //// General
    return !removedConnectionCallbackSlot.called &&
            !generateKeyPairsCallbackSlot.called &&
            !generateP2PKHAddressesCallbackSlot.called &&
            //// Buying
            !broadcastTransactionCallbackSlot.called &&
            !fullPieceArrivedCallbackSlot.called &&
             //// Selling
            !loadPieceForBuyerCallbackSlot.called &&
            !claimLastPaymentCallbackSlot.called &&
            !anchorAnnouncedCallbackSlot.called;
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::noConnectionEventsExcept(const ConnectionIdType & id) const {

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
    removedConnectionCallbackSlot.reset();
    generateKeyPairsCallbackSlot.reset();
    generateP2PKHAddressesCallbackSlot.reset();

    //// Buying
    broadcastTransactionCallbackSlot.reset();
    fullPieceArrivedCallbackSlot.reset();

    //// Selling
    loadPieceForBuyerCallbackSlot.reset();
    claimLastPaymentCallbackSlot.reset();
    anchorAnnouncedCallbackSlot.reset();

    // Connection spies
    for(auto mapping : connectionSpies)
        mapping.second->reset();
}
