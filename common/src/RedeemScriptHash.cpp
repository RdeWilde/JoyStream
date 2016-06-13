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

RedeemScriptHash::RedeemScriptHash(const Script &script)
    : RedeemScriptHash(script.serialize()) {

}

RedeemScriptHash::RedeemScriptHash(const uchar_vector &script)
    : UCharArray(ripemd160(sha256(script))) {
}

RedeemScriptHash RedeemScriptHash::fromRawHash(const uchar_vector &hash) {
    RedeemScriptHash rsh;
    rsh.setHash(hash);
    return rsh;
}

void RedeemScriptHash::setHash(const uchar_vector &hash) {
    *this = hash;
}

}

