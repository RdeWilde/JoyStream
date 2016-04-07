/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#include <protocol/statemachine/WaitingForPayment.hpp>
#include <protocol/statemachine/ReadyForPieceRequest.hpp>

#include <iostream>

using namespace joystream::protocol::statemachine;

WaitingForPayment::WaitingForPayment() {
    std::cout << "Entering WaitingForPayment state." << std::endl;
}

sc::result WaitingForPayment::react(const event::Recv<wire::Payment> & e) {

    std::cout << "Reacting to Recv<wire::Payment>." << std::endl;

    // Get payment signature
    Coin::Signature payment = e.message()->sig();

    // Check validity of payment signature, and register if valid
    bool valid = context<CBStateMachine>()._payee.registerPayment(payment);

    if(valid) {

        // Notify client about valid payment
        context<CBStateMachine>().validPayment()(payment);

        return transit<ReadyForPieceRequest>();
    } else {

        // Notify client about bad payment
        context<CBStateMachine>().invalidPayment()(payment);

        // Terminate machine
        return terminate();
    }
}
