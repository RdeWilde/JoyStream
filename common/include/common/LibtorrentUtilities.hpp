/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 18 2015
 */

#ifndef LIBTORRENT_UTILITIES_HPP
#define LIBTORRENT_UTILITIES_HPP

#include <libtorrent/socket.hpp> // libtorrent::tcp::endpoint
unsigned int qHash(const libtorrent::tcp::endpoint & endpoint);

//#include <libtorrent/peer_connection.hpp> // libtorrent::peer_request
//uint qHash(const libtorrent::peer_request & request);

#include <libtorrent/sha1_hash.hpp>

// custom specialization of std::hash can be injected in namespace std
namespace std
{
    template<> struct hash<libtorrent::sha1_hash>
    {
        typedef libtorrent::sha1_hash argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const
        {
            return std::hash<std::string>{}(s.to_string());
        }
   };
}

#endif // LIBTORRENT_UTILITIES_HPP

