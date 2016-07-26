/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 4 2015
 */

#include <protocol_wire/MessageType.hpp>
#include <cassert>
#include <string>
#include <stdexcept>

namespace joystream {
namespace protocol_wire {

    const char * messageName(MessageType type) {

        switch(type){
            case MessageType::observe: return "observe";
            case MessageType::buy: return "buy";
            case MessageType::sell: return "sell";
            case MessageType::join_contract: return "join_contract";
            case MessageType::joining_contract: return "joining_contract";
            case MessageType::sign_refund: return "sign_refund";
            case MessageType::refund_signed: return "refund_signed";
            case MessageType::ready: return "ready";
            case MessageType::request_full_piece: return "request_full_piece";
            case MessageType::full_piece: return "full_piece";
            case MessageType::payment: return "payment";
            default:
                 assert(false);
        }
    }

    MessageType messageType(const std::string & name) {

        if(messageName(MessageType::observe) == name)
            return MessageType::observe;
        else if(messageName(MessageType::buy) == name)
            return MessageType::buy;
        else if(messageName(MessageType::sell) == name)
            return MessageType::sell;
        else if(messageName(MessageType::join_contract) == name)
            return MessageType::join_contract;
        else if(messageName(MessageType::joining_contract) == name)
            return MessageType::joining_contract;
        else if(messageName(MessageType::sign_refund) == name)
            return MessageType::sign_refund;
        else if(messageName(MessageType::refund_signed) == name)
            return MessageType::refund_signed;
        else if(messageName(MessageType::ready) == name)
            return MessageType::ready;
        else if(messageName(MessageType::request_full_piece) == name)
            return MessageType::request_full_piece;
        else if(messageName(MessageType::full_piece) == name)
            return MessageType::full_piece;
        else if(messageName(MessageType::payment) == name)
            return MessageType::payment;
        else
            throw std::runtime_error("Invalid");
    }
}
}
