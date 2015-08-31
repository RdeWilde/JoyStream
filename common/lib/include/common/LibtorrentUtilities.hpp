/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 18 2015
 */

#ifndef LIBTORRENT_UTILITIES_HPP
#define LIBTORRENT_UTILITIES_HPP


#include <libtorrent/socket.hpp> // libtorrent::tcp::endpoint
uint qHash(const libtorrent::tcp::endpoint & endpoint);

//#include <libtorrent/peer_connection.hpp> // libtorrent::peer_request
//uint qHash(const libtorrent::peer_request & request);


#endif // LIBTORRENT_UTILITIES_HPP

