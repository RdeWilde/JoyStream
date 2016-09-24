/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>
 */

#include <common/RedeemScriptHash.hpp>
#include <CoinCore/hash.h>

namespace Coin {

RedeemScriptHash::RedeemScriptHash()
    : UCharArray() {
}

RedeemScriptHash::RedeemScriptHash(const Script &script) {
    setHash(ripemd160(sha256(script.serialize())));
}

RedeemScriptHash RedeemScriptHash::fromRawHash(const uchar_vector &hash) {
    RedeemScriptHash rsh;
    rsh.setHash(hash);
    return rsh;
}

RedeemScriptHash RedeemScriptHash::fromRawScript(const uchar_vector &script) {
    uchar_vector hash(ripemd160(sha256(script)));
    return fromRawHash(hash);
}

void RedeemScriptHash::setHash(const uchar_vector &hash) {
    *(UCharArray<20>*)this = hash;
}

}

