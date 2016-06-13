/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>
 */

#include <common/RedeemScriptHash.hpp>
#include <CoinCore/hash.h>

namespace Coin {

RedeemScriptHash::RedeemScriptHash()
    : UCharArray() {
}

RedeemScriptHash::RedeemScriptHash(const uchar_vector &script)
    : UCharArray(ripemd160(sha256(script))) {
}

}

