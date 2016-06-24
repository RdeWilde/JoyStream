/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/ExtendedMessageIdMapping.hpp>
#include <libtorrent/bdecode.hpp>
#include <vector>
#include <set>
#include <map>
#include <exception>

namespace joystream {
namespace extension {

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

    ExtendedMessageIdMapping::ExtendedMessageIdMapping(const libtorrent::bdecode_node & m) {

        // Iterate dictionary and copy mappings
        for(int i = 0;i < m.dict_size();i++) {

            // Recover i´th mapping
            std::pair<std::string, libtorrent::bdecode_node> mapping = m.dict_at(i);

            /**
             * The reason we do not break if we get a mismatch is
             * because all m keys will contain mappings not for
             * our extension, so we have to go through it all and
             * recover all mappings which pertain to us.
             */

            try {

                // Try to convert string to message type
                joystream::protocol_wire::MessageType message = joystream::protocol_wire::messageType(mapping.first);

                // Try to get value, skip if its not an integer
                if(mapping.second.type() != libtorrent::bdecode_node::int_t)
                    return;

                boost::int64_t value = mapping.second.int_value();

                // It worked, so store id in mapping
                _mapping[message] = value;

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

    void ExtendedMessageIdMapping::writeToDictionary(std::map<std::string, libtorrent::entry> & m) {

        // Iterate mappings and dump to dictionary
        for(std::map<joystream::protocol_wire::MessageType, quint8>::const_iterator i = _mapping.begin(),
            end(_mapping.end()); i != end;i++)
            m[joystream::protocol_wire::messageName(i->first)] = i->second;
    }

    quint8 ExtendedMessageIdMapping::id(joystream::protocol_wire::MessageType messageType) const {

        // Search for message
        std::map<joystream::protocol_wire::MessageType, quint8>::const_iterator i = _mapping.find(messageType);

        // Throw exception if no match, otherwise return id
        if(i == _mapping.end())
            throw std::runtime_error("Incomplete mapping, no id for message type.");
        else
            return i->second;
    }

    void ExtendedMessageIdMapping::id(joystream::protocol_wire::MessageType messageType, quint8 id) {

        // Add mapping if not there, substitute if it is
        _mapping[messageType] = id;
    }

    // Get message corresponding to id
    joystream::protocol_wire::MessageType ExtendedMessageIdMapping::messageType(quint8 id) const {

        // Find all message types mappings to given id, should be exactly one
        std::vector<joystream::protocol_wire::MessageType> found;

        for(std::map<joystream::protocol_wire::MessageType, quint8>::const_iterator i = _mapping.begin(),
            end(_mapping.end()); i != end;i++) {

            // Rememeber if id matches
            if(i->second == id)
                found.push_back(i->first);
        }

        // If there was note exactly one
        if(found.size() > 1)
            throw std::runtime_error("Invalid mapping, repeating id.");
        else if(found.size() < 1)
            throw std::runtime_error("Mapping does not contain id."); // May be invalid mapping, or invalid id
        else
            return found.front(); // Return message found
    }

    // Check validity of mapping
    bool ExtendedMessageIdMapping::isValid() const {

        // Get the number of messages
        uint numberOfMessages = joystream::protocol_wire::numberOfMessageTypes();

        // Has the right number of mappings
        if(_mapping.size() != numberOfMessages)
            return false;

        // Check if all ids are unique
        std::set<quint8> uniqueIds;

        for(std::map<joystream::protocol_wire::MessageType, quint8>::const_iterator i = _mapping.begin(),
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
        const std::set<joystream::protocol_wire::MessageType> allMessageTypes = joystream::protocol_wire::allMessageTypes();

        // Create mapping for all messages, starting at _id
        for(std::set<joystream::protocol_wire::MessageType>::const_iterator i = allMessageTypes.begin(),
            end(allMessageTypes.end()); i != end;i++)
                id(*i, _id++);
    }

    std::map<joystream::protocol_wire::MessageType, quint8> ExtendedMessageIdMapping::mapping() const {
        return _mapping;
    }

    void ExtendedMessageIdMapping::setMapping(const std::map<joystream::protocol_wire::MessageType, quint8> & mapping) {
        _mapping = mapping;
    }
}
}
