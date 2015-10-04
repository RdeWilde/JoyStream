/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 4 2015
 */

#include <core/extension/Message/MessageType.hpp>

#include <QtGlobal> // Q_ASSERT

const char * MessageTypeToString(MessageType type) {

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
             Q_ASSERT(false);
    }
}
