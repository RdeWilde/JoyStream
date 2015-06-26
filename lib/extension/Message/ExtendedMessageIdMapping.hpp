/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef EXTENDED_MESSAGE_ID_MAPPING_HPP
#define EXTENDED_MESSAGE_ID_MAPPING_HPP

#include "MessageType.hpp"

//#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/entry.hpp>

#include <QtGlobal> // quint8

class ExtendedMessageIdMapping
{

public:

    // Default constructor
    ExtendedMessageIdMapping();

    // Constructor from extended hanshake dictionary m, used when parsing hanshake from peer
    ExtendedMessageIdMapping(const std::map<std::string, libtorrent::entry> & m);

    // Assignment operator
    ExtendedMessageIdMapping & operator=(const ExtendedMessageIdMapping & rhs);

    // Outputs mapping into and m dicitonary, which can be used in extended handshake
    void writeToDictionary(std::map<std::string, libtorrent::entry> & m);

    // Get extended message id of given message
    quint8 id(MessageType messageType) const;

    // Set extended message id of given message
    void id(MessageType messageType, quint8 id);

    // Get message corresponding to id
    MessageType messageType(quint8 id) const;

    // Check that mapping is valid
    bool isValid() const;

    // Sets all message ids starting at the given parameter
    void setAllStartingAt(quint8 _id);

    // Get underlying std::map
    std::map<MessageType, quint8> mapping() const;

    // Set underlying std::map
    void setMapping(const std::map<MessageType, quint8> &mapping);

private:

    // Message to id mapping
    std::map<MessageType, quint8> _mapping;
};

#endif // EXTENDED_MESSAGE_ID_MAPPING_HPP
