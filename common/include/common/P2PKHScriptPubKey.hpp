/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 3 2015
 */

#ifndef P2PKH_SCRIPT_PUBKEY_HPP
#define P2PKH_SCRIPT_PUBKEY_HPP

#include <common/PubKeyHash.hpp>

namespace Coin {

class PublicKey;

/** NOTE: Make subclass of proper typesafe script class in the future **/
class P2PKHScriptPubKey {

public:

    P2PKHScriptPubKey(const PubKeyHash & hash);

    P2PKHScriptPubKey(const PublicKey & pk);

    // From raw script
    static P2PKHScriptPubKey deserialize(const uchar_vector & script);
    static P2PKHScriptPubKey deserialize(const std::string & hex);

    // Raw script: scriptSig: <sig> <pubKey>
    uchar_vector serialize() const;

    PubKeyHash pubKeyHash() const { return _hash; }

private:

    PubKeyHash _hash;
};

}

#endif // P2PKH_SCRIPT_PUBKEY_HPP

