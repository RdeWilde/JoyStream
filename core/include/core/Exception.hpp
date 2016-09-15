/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 16 2016
 */

#ifndef JOYSTREAM_CORE_EXCEPTION_HPP
#define JOYSTREAM_CORE_EXCEPTION_HPP

#include <core/Node.hpp>

#include <exception>
#include <QJsonValue>

namespace joystream {
namespace core {
namespace exception {

    class TorrentAlreadyExists : public std::runtime_error {

    public:

        TorrentAlreadyExists(const libtorrent::sha1_hash & infoHash)
            : std::runtime_error(toMessage(infoHash))
            , infoHash(infoHash) {
        }

        libtorrent::sha1_hash infoHash;

    private:

        static std::string toMessage(const libtorrent::sha1_hash & infoHash) {
            return std::string("Torrent already present with same info_hash: ") + infoHash.to_string();
        }
    };

    class FailedToStartNodeException : public std::runtime_error {

    public:

        FailedToStartNodeException(const libtorrent::error_code & ec)
            : std::runtime_error("Failed to start node: " + ec.message())
            , _ec(ec) {
        }

        libtorrent::error_code ec() const noexcept {
            return _ec;
        }

    private:

        libtorrent::error_code _ec;

    };

    class HandleNotSet : public std::runtime_error {

    public:

        HandleNotSet()
            : std::runtime_error("Handle is not set") {
        }
    };

    class HandleExpired : public std::runtime_error {

    public:

        HandleExpired()
            : std::runtime_error("Handle has expired") {
        }
    };
}
}
}

#endif // JOYSTREAM_CORE_EXCEPTION_HPP
