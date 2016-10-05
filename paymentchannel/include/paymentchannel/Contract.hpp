/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 20 2016
 */

#ifndef PAYMENTCHANNEL_CONTRACT_HPP
#define PAYMENTCHANNEL_CONTRACT_HPP

#include <common/UnspentOutputSet.hpp>
#include <common/Payment.hpp>
#include <paymentchannel/Commitment.hpp>

#include <vector>

namespace Coin {
    class Transaction;
}

namespace joystream {
namespace paymentchannel {

class Contract {

public:

    Contract();

    Contract(const Coin::UnspentOutputSet &);

    // Setup contract without change
    Contract(const Coin::UnspentOutputSet &,
             const std::vector<Commitment> &);

    // Setup contract with change
    Contract(const Coin::UnspentOutputSet &,
             const std::vector<Commitment> &,
             const Coin::Payment &);

    // Adds commitment, and returns number of commitments in total after adding
    uint addCommitment(const Commitment &);

    // Set change
    void setChange(const Coin::Payment &);

    // Removes the change
    void clearChange();

    // The transaction corresponding to the contract
    Coin::Transaction transaction() const;

    // Transaction fee for contract with given terms
    static uint64_t fee(uint32_t numberOfCommitments, bool hasChange, quint64 feePerKb, int numberOfInputs = 1);

private:

    // The size of a contract transaction with given terms
    static uint32_t transactionSize(uint32_t, bool, int);

    // Funding contract
    Coin::UnspentOutputSet _funding;

    // Commitments for end to end channels
    std::vector<Commitment> _commitments;

    // Change in contract back to payor
    // NB: ** replace with std::optional<> when it comes out
    bool _changeSet;
    Coin::Payment _change;
};

}
}

#endif // PAYMENTCHANNEL_CONTRACT_HPP
