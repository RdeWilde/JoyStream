/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 16 2016
 */

#ifndef JOYSTREAM_CORE_DETAIL_TORRENT_HPP
#define JOYSTREAM_CORE_DETAIL_TORRENT_HPP

/**
#include <extension/extension.hpp>
#include <core/Callbacks.hpp>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <cstdint>
*/

#include <map>
#include <memory>

namespace joystream {
namespace core {
namespace detail {

// Strip out std::unique_ptr instances from map and replace with raw pointers
template <class KEY, class VAL>
std::map<KEY, VAL *> getRawMap(const std::map<KEY, std::unique_ptr<VAL>> & map) {

    std::map<KEY, VAL *> strippedMap;

    for(auto & mapping : map)
        strippedMap.insert(std::make_pair(mapping.first, mapping.second.get()));

    return strippedMap;
}

}
}
}

#endif // JOYSTREAM_CORE_DETAIL_TORRENT_HPP

