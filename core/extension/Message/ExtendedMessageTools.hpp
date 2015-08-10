#ifndef EXTENDED_MESSAGE_TOOLS_HPP
#define EXTENDED_MESSAGE_TOOLS_HPP

#include "MessageType.hpp"

#include <set>
#include <map>

namespace ExtendedMessageTools {

// Mapping from type to name
static const std::map<MessageType, const char *> messageTypeToNameMapping();

// Name from type
static const char * messageName(MessageType type);

// Type from name
static MessageType messageType(const std::string & messageName);

// All messages
static const std::set<MessageType> allMessageTypes();

// Count the number of message types
static int numberOfMessageTypes();

}

#endif // EXTENDED_MESSAGE_TOOLS_HPP

