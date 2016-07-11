/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 16 2016
 */

#ifndef JOYSTREAM_CORE_DETAIL_TORRENT_HPP
#define JOYSTREAM_CORE_DETAIL_TORRENT_HPP

#include <extension/extension.hpp>
#include <core/Callbacks.hpp>
#include <core/Configuration.hpp>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/torrent_handle.hpp>

#include <cstdint>

namespace joystream {
namespace core {
namespace detail {

/**
// An unsynchronized counter shared across multiple callbacks.
class SharedCounter {

public:

    SharedCounter()
        : SharedCounter(0) {
    }

    SharedCounter(uint initialValue)
        : _count(new uint) {

        *(_count.get()) = initialValue;
    }

    uint increment() const {

        uint * ptr = _count.get();

        (*ptr)++;

        return (*ptr);
    }

    void decrement() const {

        uint * ptr = _count.get();

        if((*ptr) == 0)
            throw std::runtime_error("Counter already depleted.");
        else
            (*ptr)--;
    }

    uint count() const {
        return *(_count.get());
    }

    bool zero() const {
        return count() == 0;
    }

private:

    // Shared underlying count
    std::shared_ptr<uint> _count;
};
*/

struct AddTorrentCall {

    AddTorrentCall(const AddedTorrent & addedTorrent,
                   const configuration::Torrent & configuration)
        : addedTorrent(addedTorrent)
        , configuration(configuration) {
    }

    AddedTorrent addedTorrent;
    configuration::Torrent configuration;
};

}
}
}

#endif // JOYSTREAM_CORE_DETAIL_TORRENT_HPP

