/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 2 2015
 */

#include <common/RedeemScriptHash.hpp>
#include <common/PublicKey.hpp>
#include <common/Utilities.hpp> //  Coin::uchar_vector opPushData(uint32_t nBytes)
#include <CoinCore/hash.h> // ripemd160(sha256(*))

namespace Coin {

RedeemScriptHash::RedeemScriptHash(const uchar_vector & raw)
    : UCharArray<REDEEM_SCRIPT_HASH_BYTE_LENGTH>(raw) {
}

RedeemScriptHash RedeemScriptHash::multisig(const std::vector<PublicKey> & keys, uint mininumNumberOfSignatures) {

    // Copied from Script::Script(type_t type, unsigned int minsigs, const std::vector<bytes_t>& pubkeys, const std::vector<bytes_t>& sigs)

    // Check inputs are compatible
    if (mininumNumberOfSignatures == 0
        || keys.size() > 16
        || mininumNumberOfSignatures > keys.size())
        throw std::runtime_error("Invalid parameters.");

    // Build raw redeem script
    uchar_vector redeemscript;

    // Add m
    redeemscript.push_back(mininumNumberOfSignatures + 0x50);

    // Add each valid raw key
    for(std::vector<PublicKey>::const_iterator i = keys.cbegin(), end = keys.cend(); i != end; i++) {
        redeemscript += Coin::opPushData(PublicKey::length());
        redeemscript += (*i).toUCharVector();
    }

    // Add n
    redeemscript.push_back(keys.size() + 0x50);

    // Checking
    redeemscript.push_back(0xae); // OP_CHECKMULTISIG

    // Create script hash and return
    return RedeemScriptHash(ripemd160(sha256(redeemscript)));
}

uchar_vector RedeemScriptHash::toScriptPubKey() const {

    // Copied from bytes_t Script::txoutscript() const

    uchar_vector script;

    script.push_back(0xa9); // OP_HASH160
    script.push_back(0x14); // push a 20 byte hash
    script += toUCharVector();
    script.push_back(0x87); // OP_EQUAL

    return script;
}

}
