/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 2 2015
 */

#include <common/PubKeyHash.hpp>
#include <CoinCore/CoinNodeData.h> // TxOut

namespace Coin {

PubKeyHash::PubKeyHash()
    : UCharArray<PUBKEY_HASH_BYTE_LENGTH>() {
}

PubKeyHash::PubKeyHash(const uchar_vector & raw)
    : UCharArray<PUBKEY_HASH_BYTE_LENGTH>(raw) {
}

uchar_vector PubKeyHash::toScriptPubKey() const {

    // Copied from bytes_t Script::txoutscript() const

    uchar_vector script;

    script.push_back(0x76); // OP_PUSHDATA1
    script.push_back(0xa9); // OP_HASH160
    script.push_back(0x14); // push a 20 byte hash
    script += toUCharVector(); // pubkeyhash
    script.push_back(0x88); // OP_EQUALVERIFY
    script.push_back(0xac); // OP_CHECKSIG

    return script;
}

}
