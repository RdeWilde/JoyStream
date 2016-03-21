/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#include <protocol/statemachine/WaitingForPayment.hpp>

#include <iostream>

using namespace joystream::protocol::statemachine;

WaitingForPayment::WaitingForPayment() {
    std::cout << "Entering WaitingForPayment state." << std::endl;
}

sc::result WaitingForPayment::react(const event::Recv<wire::Payment> &) {

    std::cout << "Reacting to event::Recv<wire::Paymen>." << std::endl;

    // NOT DONE

    /**
    if(valid) {
        // transition to ReadyForPieceRequest
        // we need to increment payment counter in the selling class
        //
    } else {
        // send invlaidPayment to lcient
        //  terminate
    }
    */
}
