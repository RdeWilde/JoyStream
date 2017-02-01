/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 31 2017
 */

#include <libtorrent/socket_io.hpp>
#include <functional>

namespace std {

// hash<libtorrent::tcp::endpoint> needed for std::unordered map with this template key
template<>
struct hash<libtorrent::tcp::endpoint> {
    size_t operator()(const libtorrent::tcp::endpoint & pt) const;
};

}
