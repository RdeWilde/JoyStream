/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_EXTENSION_EXTENDED_MESSAGE_ID_MAPPING_HPP
#define JOYSTREAM_EXTENSION_EXTENDED_MESSAGE_ID_MAPPING_HPP

#include <protocol_wire/protocol_wire.hpp>

#include <libtorrent/entry.hpp>

#include <QtGlobal> // quint8

namespace joystream {

namespace protocol_wire {
    enum class MessageType;
}

namespace extension {

    class ExtendedMessageIdMapping {

    public:

        // Default constructor
        ExtendedMessageIdMapping();

        // Constructor from extended hanshake dictionary m, used when parsing hanshake from peer
        ExtendedMessageIdMapping(const libtorrent::bdecode_node & m);

        // Assignment operator
        ExtendedMessageIdMapping & operator=(const ExtendedMessageIdMapping & rhs);

        // Outputs mapping into and m dicitonary, which can be used in extended handshake
        void writeToMDictionary(std::map<std::string, libtorrent::entry> & m);

        // Get extended message id of given message
        quint8 id(joystream::protocol_wire::MessageType messageType) const;

        // Set extended message id of given message
        void id(joystream::protocol_wire::MessageType messageType, quint8 id);

        // Get message corresponding to id
        joystream::protocol_wire::MessageType messageType(quint8 id) const;

        // Check that mapping is valid
        bool isValid() const;

        // Sets all message ids starting at the given parameter
        void setAllStartingAt(quint8 _id);

        // Get underlying std::map
        std::map<joystream::protocol_wire::MessageType, quint8> mapping() const;

        // Set underlying std::map
        void setMapping(const std::map<joystream::protocol_wire::MessageType, quint8> & mapping);

    private:

        // Message to id mapping
        std::map<joystream::protocol_wire::MessageType, quint8> _mapping;
    };

}
}

#endif // JOYSTREAM_EXTENSION_EXTENDED_MESSAGE_ID_MAPPING_HPP
