/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

#include <protocol/statemachine/CBStateMachine.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/KeyPair.hpp>

namespace joystream {
namespace protocol {
namespace wire {
    class ExtendedMessagePayload;
}
}
}

using namespace joystream::protocol;

class Test : public QObject {

    Q_OBJECT

private slots:

    // Before any test
    void initTestCase();

    // Before each test
    void init();

    // ***
    // Actual tests
    // ***

    // Lots of state transitions
    void clientToSellMode();
    void clientToBuyMode();
    void clientToObserveMode();

    //
    void peerToSellMode();
    void peerToBuyMode();
    void peerToObserveMode();

public:

    // ***
    // Variables for encoding result of most recent callback
    // ***

    // InvitedToOutdatedContract
    statemachine::CBStateMachine::InvitedToOutdatedContract _invitedToOutdatedContract;
    bool _hasBeenInvitedToOutdatedContract;

    // InvitedToJoinContract
    statemachine::CBStateMachine::InvitedToJoinContract _invitedToJoinContract;
    bool _hasBeenInvitedToJoinContract;
    Coin::typesafeOutPoint _anchor;
    int64_t _funds;
    Coin::PublicKey _contractPk;

    // Send
    statemachine::CBStateMachine::Send _send;
    bool _messageSent;
    const wire::ExtendedMessagePayload * _sendMessage;

    // ContractIsReady
    statemachine::CBStateMachine::ContractIsReady _contractIsReady;
    bool _contractHasBeenPrepared;
    Coin::typesafeOutPoint _readyContract;

    // PieceRequested
    statemachine::CBStateMachine::PieceRequested _pieceRequested;
    bool _pieceHasBeenRequested;
    int _piece;

private:

    // Utility routines
    statemachine::CBStateMachine * createFreshMachineInObserveMode();
    statemachine::CBStateMachine * createFreshMachineInBuyMode(const BuyerTerms & terms);
    statemachine::CBStateMachine * createFreshMachineInSellMode(const SellerTerms & terms);
    statemachine::CBStateMachine * createFreshMachine();

    void resetCallbackState();
};

#endif // TEST_HPP
