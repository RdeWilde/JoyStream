/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Message/ExtendedMessageTools.hpp>

namespace ExtendedMessageTools {

const std::map<MessageType, const char *> messageTypeToNameMapping() {

    // Create type to name mapping
    std::map<MessageType, const char *> m;

    // Setup mapping
    m[MessageType::observe] = "observe";
    m[MessageType::buy] = "buy";
    m[MessageType::sell] = "sell";
    m[MessageType::join_contract] = "join_contract";
    m[MessageType::joining_contract] = "joining_contract";
    m[MessageType::sign_refund] = "sign_refund";
    m[MessageType::refund_signed] = "refund_signed";
    m[MessageType::ready] = "ready";
    m[MessageType::request_full_piece] = "request_full_piece";
    m[MessageType::full_piece] = "full_piece";
    m[MessageType::payment] = "payment";


    // Return mapping
    return m;
}

const char * messageName(MessageType type) {

    // Get mapping
    const std::map<MessageType, const char *> m = messageTypeToNameMapping();

    // Find name of type
    std::map<MessageType, const char *>::const_iterator i = m.find(type);

    if(i == m.end())
        throw std::runtime_error("No name registered for type, messageTypeToNameMapping() does not contain all types!");
    else
        return i->second;
}

MessageType messageType(const std::string & messageName) {

    // Invert messageTypeToNameMapping()

    // Get type to name mapping
    const std::map<MessageType, const char *> m = messageTypeToNameMapping();

    // Iterate map and find preimage of messageName
    for(std::map<MessageType, const char *>::const_iterator i = m.begin(),
            end(m.end()); i != end;i++) {

        // return type if name matches
        if(messageName.compare(i->second) == 0)
            return i->first;
    }

    throw std::runtime_error("Unknown message name passed.");
}

const std::set<MessageType> allMessageTypes() {

    // Create set
    std::set<MessageType> m;

    // Populate set by taking keys from messageTypeToNameMapping()
    std::map<MessageType, const char *> x = messageTypeToNameMapping();

    // Iterate map and find preimage of messageName
    for(std::map<MessageType, const char *>::const_iterator i = x.begin(),
            end(x.end()); i != end;i++)
        m.insert(i->first);

    // Return mappings
    return m;
}

int numberOfMessageTypes() {
    return allMessageTypes().size();
}

}
