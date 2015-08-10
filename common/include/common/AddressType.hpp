/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 8 2015
 */

#ifndef ADDRESS_TYPE_HPP
#define ADDRESS_TYPE_HPP

namespace Coin {

/**
 * Type of output address
 */

enum class AddressType {
    PayToPublicKeyHash,
    //PayToPublicKey,
    PayToScriptHash
    //Multisig,

    //Data // Does this have address format
};

}

#endif // ADDRESS_TYPE_HPP
