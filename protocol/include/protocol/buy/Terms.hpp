/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUY_TERMS_HPP
#define JOYSTREAM_PROTOCOL_BUY_TERMS_HPP

#include <stdint.h>

namespace joystream {
namespace protocol {
namespace buy {

    // Terms in buy mode message
    struct Terms {

        // Maximum price accepted (satoshies)
        int64_t _maxPrice;

        // Maximum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
        uint32_t _maxLock;

        // Maximum fee per byte in contract transaction (satoshies)
        int64_t _maxFeePerKb;

        // Number of sellers
        uint32_t _numberOfSellers;
    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_BUY_TERMS_HPP
