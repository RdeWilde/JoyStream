/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_BUYER_TERMS_HPP
#define JOYSTREAM_PROTOCOL_BUYER_TERMS_HPP

//#include <stdint.h>
#include <QtGlobal>

namespace joystream {
namespace protocol {

    // Terms in buy mode message
    struct BuyerTerms {

        // Maximum price accepted (satoshies)
        //int64_t _maxPrice;
        quint64 _maxPrice;

        // Maximum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
        //uint32_t _maxLock;
        quint32 _maxLock;

        // Maximum fee per byte in contract transaction (satoshies)
        //int64_t _maxFeePerKb;
        quint64 _maxFeePerKb;

        // Number of sellers
        //uint32_t _numberOfSellers;
        quint32 _numberOfSellers;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_BUYER_TERMS_HPP
