/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 24 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_PAYOR_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_PAYOR_HPP

#include <QObject>

namespace Coin {
    class typesafeOutPoint;
}

namespace joystream {
namespace core {
    class Payor;
}
namespace classic {
namespace controller {

class Payor : public QObject {

    Q_OBJECT

public:

    Payor(core::Payor * payor);

public slots:

    void setPrice(quint64);

    void setNumberOfPaymentsMade(quint64);

    void setFunds(quint64);

    void setSettlementFee(quint64);

    void setRefundLockTime(quint32);

    void setAnchorChanged(const Coin::typesafeOutPoint &);

private:

    core::Payor * _payor;


};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_PAYOR_HPP
