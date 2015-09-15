/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#include <common/P2PKHScriptSig.hpp>
#include <common/Utilities.hpp> // opPushData

namespace Coin {

P2PKHScriptSig::P2PKHScriptSig(const PublicKey & pk, const TransactionSignature & ts)
    : _pk(pk)
    , _ts(ts) {
}

uchar_vector P2PKHScriptSig::serialized() const {

    uchar_vector serialized;

    // Add signature+sighash type
    serialized += _ts.opPushForScriptSigSerialized();

    // Add public key length
    serialized += opPushData(PublicKey::length());

    // Add public key
    serialized += _pk.toUCharVector();

    return serialized;

}

PublicKey P2PKHScriptSig::pk() const {
    return _pk;
}

void P2PKHScriptSig::setPk(const PublicKey & pk) {
    _pk = pk;
}

TransactionSignature P2PKHScriptSig::ts() const {
    return _ts;
}

void P2PKHScriptSig::setTs(const TransactionSignature & ts) {
    _ts = ts;
}

}
