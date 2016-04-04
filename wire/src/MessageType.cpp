/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 4 2015
 */

#include <wire/MessageType.hpp>

#include <QtGlobal> // Q_ASSERT

#include <vector>
#include <string>

namespace joystream {
namespace wire {

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
}
