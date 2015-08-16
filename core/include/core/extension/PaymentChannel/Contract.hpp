/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#ifndef CONTRACT_HPP
#define CONTRACT_HPP

//#include <CoinCore/StandardTransactions.h>
#include <core/extension/PaymentChannel/Commitment.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <common/Payment.hpp>

#include <QVector>

namespace Coin {
    //class UnspentP2PKHOutput;
    //class P2SHTxOut;
    //class P2PKHTxOut;
    class Transaction;
}

//class QVector<Coin::OLD_P2SHTxOut>;

// Payment channel contract
// NB: Not sure if this really needs to be a class,
// if it turns out that you never really need to persist
// such an object, then just turn into a function in Payor
/**
 * I DONT THINK THIS CLASS IS OF MUCH USE, ABSORB INTO PAYOR IN FUTURE?
 */
class Contract  {

public:

    // Constructor
    Contract(const Coin::UnspentP2PKHOutput & funding,
             const QVector<Coin::Commitment> & channels,
             const Coin::Payment & change);

    // Validate a raw contract
    static bool validateContractTrasaction(const Coin::Transaction & transaction);

    // The transaction corresponding to the contract
    Coin::Transaction transaction() const;

    //
    //static Coin::OLD_P2SHTxOut Contract::toP2SHTxOut();

private:

    // Funding contract
    Coin::UnspentP2PKHOutput _funding;

    // Commitments for end to end channels
    QVector<Coin::Commitment> _channelCommitments;

    // Change in contract back to payor
    Coin::Payment _change;

    // Transaction implementing contract
    // NB: Contract does not subclass Coin::Transaction,
    // as all the regular modifers would undermine the type safety
    // of the contract.
    //Coin::Transaction _transaction;
};

#endif // CONTRACT_HPP

