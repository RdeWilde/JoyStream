/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#ifndef REFUND_HPP
#define REFUND_HPP

#include <common/PublicKey.hpp>

namespace Coin {
    class OutPoint;
    class Signature;
    //class PublicKey;, cannot forward declare typedef, so we include above
    class Payment;
}

/**
 * I DONT THINK THIS CLASS IS OF MUCH USE, ABSORB INTO PAYOR IN FUTURE?
 */
// is this class really needed?
class Refund {

public:

    // Validate payee signature.
    // Is used by payor
    static bool validate(const Coin::OutPoint & outpoint,
                         const Coin::Signature & payorSig,
                         const Coin::Signature & payeeSig,
                         const Coin::PublicKey & payorPk,
                         const Coin::PublicKey & payeePk,
                         const Coin::Payment & refund,
                         quint32 lockTime);

private:

};

#endif // REFUND_HPP

