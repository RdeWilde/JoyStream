/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_EXTENDED_MESSAGE_ID_MAPPING_HPP
#define JOYSTREAM_EXTENSION_EXTENDED_MESSAGE_ID_MAPPING_HPP

#include <protocol_wire/protocol_wire.hpp>
#include <libtorrent/bdecode.hpp>
#include <libtorrent/entry.hpp>

#include <set>
#include <map>
#include <exception>

#define CLIENT_PREFIX_STRING "js_"

namespace joystream {
namespace extension {

    // A BEP10 handshake message id mapping for the crypto currency extension.
    // Mapping is always bijective (one-to-one + onto)
    class ExtendedMessageIdMapping {

    public:

        ExtendedMessageIdMapping(const ExtendedMessageIdMapping &);

        ExtendedMessageIdMapping & operator=(const ExtendedMessageIdMapping & rhs);

        // Constructor from extended hanshake dictionary m.
        // Throws: exception::InvalidMessageMappingDictionary
        static ExtendedMessageIdMapping fromMDictionary(const libtorrent::bdecode_node & m);

        // Sets all message ids starting at the given parameter
        static ExtendedMessageIdMapping consecutiveIdsStartingAt(uint8_t start);

        // Writes an uninstall mapping into (BEP10) m dictionary
        // Throws:
        // * MessageAlreadyPresentException: if dictionary already has key corresponding to this extension
        static void writeUninstallMappingToMDictionary(libtorrent::entry::dictionary_type &);

        // Writes this mapping into (BEP10) m dictionary
        // Throws:
        // * MessageAlreadyPresentException: if dictionary already has key corresponding to this extension
        void writeToMDictionary(libtorrent::entry::dictionary_type & m);

        /// Getters

        // Returns id of given message
        uint8_t id(protocol_wire::MessageType) const;

        // Returns the message with the givne id
        // Throws:
        // NoSuchIdException: if there is no message with given id
        protocol_wire::MessageType messageType(uint8_t) const;

    private:

        typedef std::map<joystream::protocol_wire::MessageType, uint8_t> RawMapping;

        // Write raw mapping to m dictionary entry
        static void writeMappingToMDictionary(const RawMapping &, libtorrent::entry::dictionary_type &);

        // Private default
        ExtendedMessageIdMapping(const RawMapping &);

        // Full set of messages used in ma
        const static std::set<protocol_wire::MessageType> messages;

        // Message to id mapping
        // Invariant: _mapping.size() == messages.size()
        RawMapping _mapping;
    };
}
}

#endif // JOYSTREAM_EXTENSION_EXTENDED_MESSAGE_ID_MAPPING_HPP
