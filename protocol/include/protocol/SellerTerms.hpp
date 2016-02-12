/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 5 2016
 */

#ifndef JOYSTREAM_PROTOCOL_SELLER_TERMS_HPP
#define JOYSTREAM_PROTOCOL_SELLER_TERMS_HPP

//#include <stdint.h>

#include <QtGlobal>

namespace joystream {
namespace protocol {

    // Terms in sell mode message
    struct SellerTerms {

        // Piece price (in satoshi units)
        //int64_t _minPrice;
        quint64 _minPrice;

        // When refund is spendable at the earliest
        //uint32_t _minLock;
        quint32 _minLock;

        // Maximum number of sellers accepted in contract
        //uint32_t _maxSellers;
        quint32 _maxSellers;

    };

}
}

#endif // JOYSTREAM_PROTOCOL_SELLER_TERMS_HPP
