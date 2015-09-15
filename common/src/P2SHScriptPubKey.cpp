/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <common/P2SHScriptPubKey.hpp>
#include <common/MultisigScriptPubKey.hpp>

namespace Coin {

P2SHScriptPubKey::P2SHScriptPubKey(const RedeemScriptHash & hash)
    : _hash(hash) {
}

P2SHScriptPubKey::P2SHScriptPubKey(const std::vector<PublicKey> & keys, uint mininumNumberOfSignatures)
    : P2SHScriptPubKey::P2SHScriptPubKey(Coin::MultisigScriptPubKey(keys, mininumNumberOfSignatures).scriptHash()) {
}

uchar_vector P2SHScriptPubKey::serialize() const {

    // Copied from bytes_t Script::txoutscript() const

    uchar_vector script;

    script.push_back(0xa9); // OP_HASH160
    script.push_back(0x14); // push a 20 byte hash
    script += _hash.toUCharVector();
    script.push_back(0x87); // OP_EQUAL

    return script;
}

}
