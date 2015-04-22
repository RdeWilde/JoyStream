#include "ExtendedMessageIdMapping.hpp"
#include "Utilities.hpp" // const char * messageName(MessageType type)

#include <vector>
#include <exception>

ExtendedMessageIdMapping::ExtendedMessageIdMapping() {

    /**
      * Starting point from where to map:
      * =================================
      * So this is a bit of a mess. First I attempted to just look at all prexisting registrations,
      * and make sure that I started after the greatest one. However, we cannot be sure that add_handshake
      * is actually called last on our extension, and since the other extensions dont give a FUCK
      * about not overwriting other peoples extensions values. The only solution is then to just start
      * on some huge value which has no other extensions above it, so this value was found by trial and error.
      *
      * Old approach which did not work due to libtorrent:
      *
      * m is extended handshake dictionary
      *
        int maxExistingID = 0;
        // Iterate m key dictionary and find the greatest ID
        for(std::map<std::string, libtorrent::entry>::iterator i = m.begin(),end(m.end());i != end;i++)
            maxExistingID = std::max((int)(((*i).second).integer()), maxExistingID);
      */

    // Set all ids from 60
    setAllStartingAt(60);
}

ExtendedMessageIdMapping::ExtendedMessageIdMapping(const std::map<std::string, libtorrent::entry> & m) {

    // Iterate dictionary and copy mappings
    for(std::map<std::string, libtorrent::entry>::const_iterator i = m.begin(),
            end(m.end()); i != end;i++) {

        try {

            // Try to convert string to message type
            MessageType message = Utilities::messageType(i->first);

            // It worked, so store id in mapping
            _mapping[message] = (i->second).integer();

        } catch(std::exception e) {
            // This was not a message for this extension
        }
    }
}

ExtendedMessageIdMapping & ExtendedMessageIdMapping::operator=(const ExtendedMessageIdMapping & rhs) {

    // Copy std::map
    _mapping = rhs.mapping();

    // Return self reference
    return *this;
}

#include <qDebug>

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
