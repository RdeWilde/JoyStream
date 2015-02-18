#include "ExtendedMessageIdMapping.hpp"
#include "Utilities.hpp" // const char * messageName(MessageType type)

#include <vector>
#include <exception>

ExtendedMessageIdMapping::ExtendedMessageIdMapping() {
}

ExtendedMessageIdMapping::ExtendedMessageIdMapping(const std::map<std::string, libtorrent::entry> & m) {

    // Iterate dictionary and copy mappings
    for(std::map<std::string, libtorrent::entry>::const_iterator i = m.begin(),
            end(m.end()); i != end;i++)
        _mapping[Utilities::messageType(i->first)] = (i->second).integer();
}

ExtendedMessageIdMapping & ExtendedMessageIdMapping::operator=(const ExtendedMessageIdMapping & rhs) {

    // Copy std::map
    _mapping = rhs.mapping();

    // Return self reference
    return *this;
}

void ExtendedMessageIdMapping::writeToDictionary(std::map<std::string, libtorrent::entry> & m) {

    // Iterate mappings and dump to dictionary
    for(std::map<MessageType, quint8>::const_iterator i = _mapping.begin(),
        end(_mapping.end()); i != end;i++)
        m[Utilities::messageName(i->first)] = i->second;
}

quint8 ExtendedMessageIdMapping::id(MessageType messageType) const {

    // Search for message
    std::map<MessageType, quint8>::const_iterator i = _mapping.find(messageType);

    // Throw exception if no match, otherwise return id
    if(i == _mapping.end())
        throw std::exception("Incomplete mapping, no id for message type.");
    else
        return i->second;
}

void ExtendedMessageIdMapping::id(MessageType messageType, quint8 id) {

    // Add mapping if not there, substitute if it is
    _mapping[messageType] = id;
}

// Get message corresponding to id
MessageType ExtendedMessageIdMapping::messageType(quint8 id) const {

    // Find all message types mappings to given id, should be exactly one
    std::vector<MessageType> found;

    for(std::map<MessageType, quint8>::const_iterator i = _mapping.begin(),
        end(_mapping.end()); i != end;i++) {

        // Rememeber if id matches
        if(i->second == id)
            found.push_back(i->first);
    }

    // If there was note exactly one
    if(found.size() > 1)
        throw std::exception("Invalid mapping, repeating id.");
    else if(found.size() < 1)
        throw std::exception("Mapping does not contain id."); // May be invalid mapping, or invalid id
    else
        return found.front(); // Return message found
}

// Check validity of mapping
bool ExtendedMessageIdMapping::isValid() const {

    // Get the number of messages
    int numberOfMessages = Utilities::numberOfMessageTypes();

    // Has the right number of mappings
    if(_mapping.size() != numberOfMessages)
        return false;

    // Check if all ids are unique
    std::set<quint8> uniqueIds;

    for(std::map<MessageType, quint8>::const_iterator i = _mapping.begin(),
        end(_mapping.end()); i != end;i++)
            uniqueIds.insert(i->second);

    // Do we have the correct number of unique ids?
    if(uniqueIds.size() != numberOfMessages)
        return false;

    // No two message have same id
    return true;
}

void ExtendedMessageIdMapping::setAllStartingAt(quint8 _id) {

    // Get the set of all messages
    const std::set<MessageType> allMessageTypes = Utilities::allMessageTypes();

    // Create mapping for all messages, starting at _id
    for(std::set<MessageType>::const_iterator i = allMessageTypes.begin(),
        end(allMessageTypes.end()); i != end;i++)
            id(*i, _id++);
}

std::map<MessageType, quint8> ExtendedMessageIdMapping::mapping() const {
    return _mapping;
}

void ExtendedMessageIdMapping::setMapping(const std::map<MessageType, quint8> &mapping) {
    _mapping = mapping;
}