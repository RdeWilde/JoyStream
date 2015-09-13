/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 2 2015
 */

#ifndef COMMON_REDEEMSCRIPTHASH_HPP
#define COMMON_REDEEMSCRIPTHASH_HPP

#include <common/UCharArray.hpp>

#define REDEEM_SCRIPT_HASH_BYTE_LENGTH 20

namespace Coin {

typedef class UCharArray<REDEEM_SCRIPT_HASH_BYTE_LENGTH> RedeemScriptHash;

}

#endif // COMMON_REDEEMSCRIPTHASH_HPP

