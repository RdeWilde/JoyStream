/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#include <core/extension/PaymentChannel/Refund.hpp>

#include <CoinCore/CoinNodeData.h> // OutPoint
#include <common/Signature.hpp>

#include <common/Payment.hpp>

bool Refund::validate(const Coin::OutPoint & outpoint,
                      const Coin::Signature & payor,
                      const Coin::Signature & payee,
                      const Coin::PublicKey & payorPk,
                      const Coin::PublicKey & payeePk,
                      const Coin::Payment & refund,
                      quint32 lockTime) {
    return true;
}
