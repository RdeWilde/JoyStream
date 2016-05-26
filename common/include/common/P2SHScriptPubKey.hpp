/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#ifndef P2SH_SCRIPT_PUBKEY_HPP
#define P2SH_SCRIPT_PUBKEY_HPP

#include <common/RedeemScriptHash.hpp>

namespace Coin {

class PublicKey;

class P2SHScriptPubKey {

public:

    explicit P2SHScriptPubKey(const RedeemScriptHash & hash);

    static P2SHScriptPubKey fromSerializedRedeemScript(const uchar_vector & redeemScript);

    static P2SHScriptPubKey fromMultisig(const std::vector<PublicKey> & keys, uint mininumNumberOfSignatures);

    // Raw p2sh multisig output script: OP_HASH160 [20-byte-hash-value] OP_EQUAL
    uchar_vector serialize() const;

    // Byte length of serialized form
    static uint32_t length();

private:

    // hash of redeem script
    RedeemScriptHash _hash;

};

}


#endif // P2SH_SCRIPT_PUBKEY_HPP

