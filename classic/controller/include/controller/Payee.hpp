/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 24 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_PAYEE_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_PAYEE_HPP

#include <QObject>

namespace Coin {
    class typesafeOutPoint;
}

namespace joystream {
namespace core {
    class Payee;
}
namespace classic {
namespace controller {

class Payee : public QObject {

    Q_OBJECT

public:

    Payee(core::Payee * payor);

public slots:

    //  the number of payments which have been successfully made
    void setNumberOfPaymentsMade(quint64);

    // Cahnged payment channel lock time
    void setLockTime(quint32);

    //  price (#satoshies) increment per payment
    void setPrice(quint64);

    //  amount (#satoshies) assigned to contract output
    void setFunds(quint64);

    // Cahnged amount (#satoshies) used in fee for settlement
    void setSettlementFee(quint64);

    // Contract outpoint from which payments originate
    void setAnchor(const Coin::typesafeOutPoint &);

private:

    core::Payee * _payee;


};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_PAYEE_HPP

