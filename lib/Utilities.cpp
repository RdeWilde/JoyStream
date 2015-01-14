#include "Utilities.hpp"
#include "extension/Message/MessageType.hpp"

#include <libtorrent/entry.hpp>
#include <libtorrent/bencode.hpp> // bdecode()

#include <fstream>
#include <vector>

bool Utilities::loadBencodedEntry(const char * fileName, libtorrent::entry & destinationEntry) {

    // Open file at the end, so we can get size
    std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ifstream::ate);

    // Get size of file
    int sizeOfFile = (int)file.tellg();

    // Rewind position
    file.seekg(0);

    // Allocate space for raw file
    std::vector<char> dataVector(sizeOfFile, 0);

    // Read entire file into vector
    file.read(&dataVector[0], dataVector.size());

    // Close file
    file.close();

    // Check that entire file was read,
    // if not, indicate failure
    if(!file)
        return false;

    // Bendecode entry
    destinationEntry = libtorrent::bdecode(dataVector.begin(), dataVector.end());

    // Indicate success
    return true;
}

bool Utilities::saveBencodedEntry(const char * fileName, const libtorrent::entry & sourceEntry) {

    // Bencode entry
    std::vector<char> bencodedEntry;
    bencode(std::back_inserter(bencodedEntry), sourceEntry);

    // Open file, overwrite if present
    std::ofstream file(fileName, std::ios::out | std::ios::binary | std::ios::trunc);

    // Save to file
    file.write(reinterpret_cast<const char*>(&bencodedEntry[0]), bencodedEntry.size());

    // Check that entire file was written
    // if not, indicate failure
    if(!file)
        return false;

    // Close file
    file.close();

    // Indicate success
    return true;
}

const std::map<MessageType, const char *> Utilities::messageTypeToNameMapping() {

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
    m[MessageType::payment] = "payment";
    m[MessageType::end] = "end";

    // Return mapping
    return m;
}

const char * Utilities::messageName(MessageType type) {

    // Get mapping
    const std::map<MessageType, const char *> m = messageTypeToNameMapping();

    // Find name of type
    std::map<MessageType, const char *>::const_iterator i = m.find(type);

    if(i == m.end())
        throw std::exception("No name registered for type, messageTypeToNameMapping() does not contain all types!");
    else
        return i->second;
}

MessageType Utilities::messageType(const std::string & messageName) {

    // Invert messageTypeToNameMapping()

    // Get type to name mapping
    const std::map<MessageType, const char *> m = messageTypeToNameMapping();

    // Iterate map and find preimage of messageName
    for(std::map<MessageType, const char *>::const_iterator i = m.begin(),
            end(m.end()); i != end;i++) {

        // return type if name matches
        if(messageName.compare(i->second))
            return i->first;
    }

    throw std::exception("Unknown message name passed.");
}

const std::set<MessageType> Utilities::allMessageTypes() {

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

int Utilities::numberOfMessageTypes() {
    return allMessageTypes().size();
}
