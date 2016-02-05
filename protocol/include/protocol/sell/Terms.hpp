/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELL_TERMS_HPP
#define JOYSTREAM_PROTOCOL_SELL_TERMS_HPP

#include <stdint.h>

namespace joystream {
namespace protocol {
namespace sell {

    // Terms in sell mode message
    struct Terms {

        // Piece price (in satoshi units)
        int64_t _minPrice;

        // When refund is spendable at the earliest
        uint32_t _minLock;

        // Maximum number of sellers accepted in contract
        uint32_t _maxSellers;

    };

}
}
}

#endif // JOYSTREAM_PROTOCOL_SELL_TERMS_HPP

