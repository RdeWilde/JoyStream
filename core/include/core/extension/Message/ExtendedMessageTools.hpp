/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 12 2015
 */

#ifndef EXTENDED_MESSAGE_TOOLS_HPP
#define EXTENDED_MESSAGE_TOOLS_HPP

#include <core/extension/Message/MessageType.hpp>

#include <set>
#include <map>

namespace ExtendedMessageTools {

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

#endif // EXTENDED_MESSAGE_TOOLS_HPP

