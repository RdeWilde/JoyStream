/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

class QByteArray;
class uchar_vector;

namespace Coin {

QByteArray toByteArray(const uchar_vector & raw);

}

#include <libtorrent/socket.hpp> // libtorrent::tcp::endpoint
uint qHash(const libtorrent::tcp::endpoint & endpoint);

//#include <libtorrent/peer_connection.hpp> // libtorrent::peer_request
//uint qHash(const libtorrent::peer_request & request);

#endif // UTILITIES_HPP
