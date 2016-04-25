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

void Test::sessionImpl() {

    RemovedConnectionCallbackHandler<std::string> removedConnectionCallbackHandler;
    GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
    GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;

    detail::SessionCoreImpl<std::string> impl(removedConnectionCallbackHandler,
                                               generateKeyPairsCallbackHandler,
                                               generateP2PKHAddressesCallbackHandler);

    std::string id = "my-new-connection";

    detail::Connection<std::string> c(id,
                               joystream::protocol_statemachine::CBStateMachine::PeerAnnouncedMode(),
                               joystream::protocol_statemachine::CBStateMachine::InvitedToOutdatedContract(),
                               joystream::protocol_statemachine::CBStateMachine::InvitedToJoinContract(),
                               joystream::protocol_statemachine::CBStateMachine::Send(),
                               joystream::protocol_statemachine::CBStateMachine::ContractIsReady(),
                               joystream::protocol_statemachine::CBStateMachine::PieceRequested(),
                               joystream::protocol_statemachine::CBStateMachine::InvalidPieceRequested(),
                               joystream::protocol_statemachine::CBStateMachine::PeerInterruptedPayment(),
                               joystream::protocol_statemachine::CBStateMachine::ValidPayment(),
                               joystream::protocol_statemachine::CBStateMachine::InvalidPayment(),
                               joystream::protocol_statemachine::CBStateMachine::SellerJoined(),
                               joystream::protocol_statemachine::CBStateMachine::SellerInterruptedContract(),
                               joystream::protocol_statemachine::CBStateMachine::ReceivedFullPiece());


    impl._connections[id] = &c;
    int newSize = impl._connections.size();

    QCOMPARE(newSize, 1);

    std::vector<detail::Connection<std::string> *> ids = impl.connectionsInState<joystream::protocol_statemachine::ChooseMode>();

    QCOMPARE(ids.size(), (unsigned long)1);
}

void Test::buying() {

    RemovedConnectionCallbackHandler<std::string> removedConnectionCallbackHandler;
    GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
    GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;
    FullPieceArrived<std::string> fullPieceArrived;
    Coin::UnspentP2PKHOutput funding;
    Buying<std::string>::Policy policy;
    protocol_wire::BuyerTerms terms;
    TorrentPieceInformation information;

    Buying<std::string> buying(removedConnectionCallbackHandler,
                               generateKeyPairsCallbackHandler,
                               generateP2PKHAddressesCallbackHandler,
                               fullPieceArrived,
                               funding,
                               policy,
                               terms,
                               information);


}

void Test::session() {

/**

    RemovedConnectionCallbackHandler<std::string> removedConnectionCallbackHandler;
    GenerateKeyPairsCallbackHandler generateKeyPairsCallbackHandler;
    GenerateP2PKHAddressesCallbackHandler generateP2PKHAddressesCallbackHandler;

    Session<std::string> s(removedConnectionCallbackHandler,
              generateKeyPairsCallbackHandler,
              generateP2PKHAddressesCallbackHandler);
*/

}

QTEST_MAIN(Test)
#include "moc_Test.cpp"
