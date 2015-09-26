/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 25 2015
 */

#ifndef BLOCKCYPHER_BLOCKEXPLORER_HPP
#define BLOCKCYPHER_BLOCKEXPLORER_HPP

#include <common/BlockId.hpp> // cannot forward declare enum

namespace Coin {

    class P2PKHAddress;
    enum class Network;
    class TransactionId;
    //class BlockId;
}

namespace BlockCypher {
namespace BlockExplorer {

    // Networks are encoded in urls as follows
    // mainnet  = btc
    // testnet3 = btc-testnet
    // blockcypher testnet = bcy
    QString urlName(Coin::Network network);

    // P2PKH address
    QString link(const Coin::P2PKHAddress & address);

    // Transacton
    QString link(const Coin::TransactionId & txId, Coin::Network network);

    // Wallet
    QString link(const QString & name);

    // Block, using block id
    QString link(const Coin::BlockId & blockId);

    // Block, using block height
    QString link(int blockHeight);

}
}

#endif // BLOCKCYPHER_BLOCKEXPLORER_HPP
