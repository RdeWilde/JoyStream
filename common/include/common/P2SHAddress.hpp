/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#ifndef COIN_P2SH_ADDRESS_HPP
#define COIN_P2SH_ADDRESS_HPP

#include <common/UCharArray.hpp>
#include <common/Network.hpp>

#define P2SH_ADDRESS_REDEEMSCRIPT_BYTE_LENGTH 20

namespace Coin {

class P2SHAddress {

public:
    const static quint32 redeemScriptLength = P2SH_ADDRESS_REDEEMSCRIPT_BYTE_LENGTH;

};

}

#endif // COIN_P2SH_ADDRESS_HPP
