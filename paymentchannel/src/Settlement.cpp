/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#include <paymentchannel/Settlement.hpp>
#include <common/P2PKHScriptPubKey.hpp>
//#include <common/PubKeyHash.hpp>
#include <common/Bitcoin.hpp> // BITCOIN_DUST_LIMIT

#include <CoinCore/CoinNodeData.h> // TxOut

Settlement::Settlement(const Coin::typesafeOutPoint & contractOutPoint,
                       const Commitment & commitment,
                       const Coin::Payment & toPayor,
                       const Coin::Payment & toPayee)
    : Termination(contractOutPoint, commitment, toPayor)
    , _includePayee(true)
    , _toPayee(toPayee) {
}

Settlement::Settlement(const Coin::typesafeOutPoint & contractOutPoint,
                       const Commitment & commitment,
                       const Coin::Payment & toPayor)
    :Termination(contractOutPoint, commitment, toPayor)
    , _includePayee(false) {
}

Settlement Settlement::dustLimitAndFeeAwareSettlement(const Coin::typesafeOutPoint & contractOutPoint,
                                                      const Commitment & commitment,
                                                      const Coin::PubKeyHash & payorKeyHash,
                                                      const Coin::PubKeyHash & payeeKeyHash,
                                                      quint64 funds,
                                                      quint64 paid,
                                                      quint64 fee) {

    if(paid > funds)
        throw std::runtime_error("Funds do not cover payment.");

    // If payments cover tx fee and dust limit, then
    // send everything above fee to payee,
    // otherwise, everything goes to miners
    if(paid > (BITCOIN_DUST_LIMIT + fee))
        return Settlement(contractOutPoint,
                          commitment,
                          Coin::Payment(funds - paid, payorKeyHash),
                          Coin::Payment(paid - fee, payeeKeyHash));
    else
        return Settlement(contractOutPoint,
                          commitment,
                          Coin::Payment(funds - paid, payorKeyHash));

}

Coin::Transaction Settlement::unSignedTransaction() const {

    // Build refund transaction
    Coin::Transaction tx = Termination::unSignedTransaction();

    // Payee output if
    if(_includePayee)
        tx.addOutput(_toPayee.txOut());

    return tx;
}

int64_t Settlement::fee() const {
    return _commitment.value() - (_toPayor.value() + (_includePayee ? _toPayee.value() : 0));
}
