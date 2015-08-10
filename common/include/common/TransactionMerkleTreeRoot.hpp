/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef TRANSACTION_MERKLE_TREE_ROOT_HPP
#define TRANSACTION_MERKLE_TREE_ROOT_HPP

#include <common/FixedUCharArray.hpp>

#define TRANSACTIONMERKLETREEROOT_BYTE_LENGTH 32

namespace Coin {

typedef FixedUCharArray<TRANSACTIONMERKLETREEROOT_BYTE_LENGTH> TransactionMerkleTreeRoot;

}

#endif // TRANSACTION_MERKLE_TREE_ROOT_HPP
