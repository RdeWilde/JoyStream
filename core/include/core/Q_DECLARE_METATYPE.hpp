/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2016
 */

#ifndef Q_DECLARE_METATYPE_HPP
#define Q_DECLARE_METATYPE_HPP

#include <QMetaType>

/// Declarations, without conflict, of use to end user which wants to
/// use types with templated Qt types, e.g QVariant.

// Payor/Payee
Q_DECLARE_METATYPE(Coin::typesafeOutPoint)

// CBStateMachine
Q_DECLARE_METATYPE(joystream::protocol_statemachine::AnnouncedModeAndTerms)
Q_DECLARE_METATYPE(joystream::core::CBStateMachine::InnerStateIndex)

// Seller
Q_DECLARE_METATYPE(joystream::protocol_session::SellerState)

// Buying
Q_DECLARE_METATYPE(Coin::UnspentOutputSet)
Q_DECLARE_METATYPE(joystream::protocol_session::BuyingPolicy)
Q_DECLARE_METATYPE(joystream::protocol_session::BuyingState)
Q_DECLARE_METATYPE(joystream::protocol_wire::BuyerTerms)
Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)
Q_DECLARE_METATYPE(Coin::Transaction)

// Selling
Q_DECLARE_METATYPE(joystream::protocol_session::SellingPolicy)
Q_DECLARE_METATYPE(joystream::protocol_wire::SellerTerms)

// Session
Q_DECLARE_METATYPE(joystream::protocol_session::SessionMode)
Q_DECLARE_METATYPE(joystream::protocol_session::SessionState)
//Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)

// PeerPlugin
Q_DECLARE_METATYPE(joystream::extension::BEPSupportStatus)

// TorrentPlugin
//Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)

// Torrent
//Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)
Q_DECLARE_METATYPE(std::vector<char>)
Q_DECLARE_METATYPE(libtorrent::torrent_status::state_t)

// Node
//Q_DECLARE_METATYPE(libtorrent::tcp::endpoint)
Q_DECLARE_METATYPE(libtorrent::sha1_hash)

#endif // Q_DECLARE_METATYPE_HPP
