/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 15 2015
 */

#ifndef COIN_PAYMENT_HPP
#define COIN_PAYMENT_HPP

#include <common/PublicKey.hpp>

namespace Coin {

// Represents a payment to a public key
class Payment {

public:

    // Default constructor
    Payment();

    // Constructor from members
    Payment(quint64 value, const PublicKey & pk);

    // Copy constructor
    Payment(const Payment & o);

    // Assignment operator
    Payment & operator=(const Payment & o);

    // Getters and setters
    quint64 value() const;
    void setValue(quint64 value);

    PublicKey pk() const;
    void setPk(const PublicKey & pk);

private:

    // Value of output
    quint64 _value;

    // Public key controlling output
    PublicKey _pk;

};


}


#endif // COIN_PAYMENT_HPP

