/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#ifndef PAYMENT_CHANNEL_CONTRACT_HPP
#define PAYMENT_CHANNEL_CONTRACT_HPP

#include <common/UnspentP2PKHOutput.hpp>
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

        // Constructor
        Contract(const Coin::UnspentP2PKHOutput & funding,
                 const std::vector<Commitment> & commitments,
                 const Coin::Payment & change);

        // Required fee for contract
        static quint64 requiredFee(int numberOfPayees, quint64 feePerKb);

        // Validate a raw contract
        //static bool validateContractTrasaction(const Coin::Transaction & transaction);

        // Adds commitment, and returns number of commitments in total after adding
        uint addCommitments(const Commitment & commitment);

        // The transaction corresponding to the contract
        Coin::Transaction transaction() const;

    private:

        // Funding contract
        Coin::UnspentP2PKHOutput _funding;

        // Commitments for end to end channels
        std::vector<Commitment> _commitments;

        // Change in contract back to payor
        Coin::Payment _change;
    };

}
}

#endif // PAYMENT_CHANNEL_CONTRACT_HPP

