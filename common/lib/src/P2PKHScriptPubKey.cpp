/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <common/P2PKHScriptPubKey.hpp>
#include <common/PublicKey.hpp>

namespace Coin {

P2PKHScriptPubKey::P2PKHScriptPubKey(const PubKeyHash & hash)
    : _hash(hash) {
}

P2PKHScriptPubKey::P2PKHScriptPubKey(const PublicKey & pk)
    : P2PKHScriptPubKey(pk.toPubKeyHash()) {
}

uchar_vector P2PKHScriptPubKey::serialize() const {

    // Copied from bytes_t Script::txoutscript() const
    uchar_vector script;

    script.push_back(0x76); // OP_PUSHDATA1
    script.push_back(0xa9); // OP_HASH160
    script.push_back(0x14); // push a 20 byte hash
    script += _hash.toUCharVector(); // pubkeyhash
    script.push_back(0x88); // OP_EQUALVERIFY
    script.push_back(0xac); // OP_CHECKSIG

    return script;
}


}
