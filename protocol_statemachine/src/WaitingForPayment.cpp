/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 20 2016
 */

#include <protocol_statemachine/WaitingForPayment.hpp>
#include <protocol_statemachine/ReadyForPieceRequest.hpp>

#include <iostream>

namespace joystream {
namespace protocol_statemachine {

    WaitingForPayment::WaitingForPayment() {
        std::cout << "Entering WaitingForPayment state." << std::endl;
    }

    sc::result WaitingForPayment::react(const event::Recv<protocol_wire::Payment> & e) {

        std::cout << "Reacting to Recv<wire::Payment>." << std::endl;

        // Get payment signature
        Coin::Signature payment = e.message().sig();

        // Check validity of payment signature, and register if valid
        bool valid = context<CBStateMachine>()._payee.registerPayment(payment);

        if(valid) {

            // Notify client about valid payment
            context<CBStateMachine>()._validPayment.enqueue(payment);

            return transit<ReadyForPieceRequest>();
        } else {

            // Notify client about bad payment
            context<CBStateMachine>()._invalidPayment.enqueue(payment);

            // Terminate machine
            return terminate();
        }
    }

}
}
