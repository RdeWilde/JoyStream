/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef MESSAGE_TYPE_HPP
#define MESSAGE_TYPE_HPP

// Extended messages part of BEP43 wire protocol
enum class MessageType {

    observe,
    buy,
    sell,
    join_contract,
    joining_contract,
    sign_refund,
    refund_signed,
    ready,
    request_full_piece,
    full_piece,
    payment //, end
};

const char * MessageTypeToString(MessageType type);

//MessageType StringToMessageType(const char * str);

#endif // MESSAGE_TYPE_HPP
