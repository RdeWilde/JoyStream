/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_WIRE_MESSAGE_TYPE_HPP
#define JOYSTREAM_WIRE_MESSAGE_TYPE_HPP

// can't forwar declare?
// http://stackoverflow.com/questions/307343/forward-declare-an-stl-container
#include <map>
#include <set>

namespace joystream {
namespace protocol_wire {

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

    // Mapping from type to name
    const std::map<MessageType, const char *> messageTypeToNameMapping();

    // Name from type
    const char * messageName(MessageType type);

    // Type from name
    MessageType messageType(const std::string & messageName);

    // All messages
    const std::set<MessageType> allMessageTypes();

    // Count the number of message types
    int numberOfMessageTypes();

}
}

#endif // JOYSTREAM_WIRE_MESSAGE_TYPE_HPP
