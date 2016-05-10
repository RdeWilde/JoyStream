/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 18 2016
 */

#include <Test.hpp>
#include <protocol_session/protocol_session.hpp>
#include <protocol_wire/protocol_wire.hpp>

using namespace joystream;
using namespace joystream::protocol_session;

template<>
std::string IdToString<std::string>(const std::string & s) {
    return s;
}

void Test::session() {

    RemovedConnectionCallbackHandler<std::string> removedConnection;
    GenerateKeyPairsCallbackHandler generateKeyPairs;
    GenerateP2PKHAddressesCallbackHandler generateP2PKHAddresses;
    BroadcastTransaction hasOutstandingPayment;
    FullPieceArrived<std::string> fullPieceArrived;
    Coin::UnspentP2PKHOutput funding;
    BuyingPolicy policy;
    protocol_wire::BuyerTerms terms;
    TorrentPieceInformation information;

    Session<std::string> s;

    s.toBuyMode(removedConnection,
                generateKeyPairs,
                generateP2PKHAddresses,
                hasOutstandingPayment,
                fullPieceArrived,
                funding,
                policy,
                terms,
                information);
    s.start();


}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
