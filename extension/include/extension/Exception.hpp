/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 7 2016
 */

#ifndef JOYSTREAM_EXTENSION_EXCEPTION_HPP
#define JOYSTREAM_EXTENSION_EXCEPTION_HPP

#include <exception>

namespace joystream {
namespace extension {
namespace exception {

    class MissingTorrent : public std::runtime_error {

    public:

        MissingTorrent()
            : std::runtime_error("Torrent missing") {
        }

    };
}
}
}

#endif // JOYSTREAM_EXTENSION_EXCEPTION_HPP
