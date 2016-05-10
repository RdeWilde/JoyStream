/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 5 2015
 */

#ifndef PAYMENT_CHANNEL_TREMINATION_HPP
#define PAYMENT_CHANNEL_TREMINATION_HPP

#include <paymentchannel/Commitment.hpp>
#include <common/typesafeOutPoint.hpp>
#include <common/Payment.hpp>

namespace Coin {
    class Transaction;
    class TransactionSignature;
    class PrivateKey;
    class SigHashType;
}

namespace joystream {
namespace paymentchannel {

    /**
     * Terminates commitment,
     * Perhaps rename to "contract",
     * and rename the present contract to something else,
     * like commitment containter?
     */

    class Termination {

    public:

        Termination(const Coin::typesafeOutPoint & contractOutPoint,
                    const Commitment & commitment,
                    const Coin::Payment & toPayor);

        // Unsigned transaction
        virtual Coin::Transaction unSignedTransaction() const;

        //
        uchar_vector sighash(Coin::SigHashType type) const;

        // Transaction signature
        Coin::TransactionSignature transactionSignature(const Coin::PrivateKey & sk) const;

        // Signed transaction with given signatures
        Coin::Transaction signedTransaction(const Coin::TransactionSignature & payorTransactionSignature,
                                            const Coin::TransactionSignature & payeeTransactionSignature) const;

        // Validate signature for unsigned transaction against public key
        bool validate(const Coin::PublicKey & pk, const Coin::Signature & sig) const;

        // Validate signature for unsigned transaction against payor public key
        bool validatePayorSignature(const Coin::Signature & sig) const;

        // Validate signature for unsigned transaction against payee public key
        bool validatePayeeSignature(const Coin::Signature & sig) const;

    protected:

        // Contract to which commitment corresponds
        Coin::typesafeOutPoint _contractOutPoint;

        // Commitment being terminated
        Commitment _commitment;

        // Payment back to payor
        Coin::Payment _toPayor;
    };

}
}

#endif // PAYMENTCHANNEL_TREMINATION_HPP

