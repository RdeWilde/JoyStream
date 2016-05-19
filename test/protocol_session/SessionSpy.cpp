/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, May 9 2016
 */

#include "SessionSpy.hpp"

//// ConnectionSpy

template <class ConnectionIdType>
ConnectionSpy<ConnectionIdType>::ConnectionSpy(const ConnectionIdType & id)
    : id(id) {
}

//// SessionSpy

template <class ConnectionIdType>
SessionSpy<ConnectionIdType>::SessionSpy(const GenerateKeyPairsCallbackHandler & kpHandler,
                                         const GenerateP2PKHAddressesCallbackHandler & p2pkhHandler,
                                         const BroadcastTransaction & broadcast)
    : generateKeyPairsCallbackSlot(kpHandler)
    , generateP2PKHAddressesCallbackSlot(p2pkhHandler)
    , broadcastTransactionCallbackSlot(broadcast){
}

template <class ConnectionIdType>
void SessionSpy<ConnectionIdType>::toMonitoredSellMode(Session<ConnectionIdType> * s,
                                                       const SellingPolicy & policy,
                                                       const protocol_wire::SellerTerms & terms) {

    s->toSellMode(removedConnectionCallbackSlot.hook(),
                  generateKeyPairsCallbackSlot.hook(),
                  generateP2PKHAddressesCallbackSlot.hook(),
                  loadPieceForBuyerCallbackSlot.hook(),
                  claimLastPaymentCallbackSlot.hook(),
                  anchorAnnouncedCallbackSlot.hook(),
                  policy,
                  terms);

}

template <class ConnectionIdType>
void SessionSpy<ConnectionIdType>::toMonitoredBuyMode(Session<ConnectionIdType> * s,
                                                      const Coin::UnspentP2PKHOutput & funding,
                                                      const BuyingPolicy & policy,
                                                      const protocol_wire::BuyerTerms & terms,
                                                      const TorrentPieceInformation & information) {
    s->toBuyMode(removedConnectionCallbackSlot.hook(),
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
}

template <class ConnectionIdType>
bool SessionSpy<ConnectionIdType>::blank() {

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


