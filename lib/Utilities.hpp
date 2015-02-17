#ifndef UTILITIES_HPP
#define UTILITIES_HPP

namespace libtorrent {
    class entry;
}

enum class MessageType;

#include <set>
#include <map>

/**
 * Static utility functions
 */
class Utilities
{
public:

    // Opens file, reads contents, debencodes into destinationEntry
    static bool loadBencodedEntry(const char * fileName, libtorrent::entry & destinationEntry);

    // Bencodes dictionary, opens file, writes to file
    static bool saveBencodedEntry(const char * fileName, const libtorrent::entry & sourceEntry);

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
};

#endif // UTILITIES_HPP
