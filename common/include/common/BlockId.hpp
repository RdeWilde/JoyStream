/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef BLOCK_ID_HPP
#define BLOCK_ID_HPP

#include <common/FixedUCharArray.hpp>

#define BLOCKID_BYTE_LENGTH 32

namespace Coin {

    typedef FixedUCharArray<BLOCKID_BYTE_LENGTH> BlockId;
}

#endif // BLOCK_ID_HPP
