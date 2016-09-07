/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 16 2016
 */

#ifndef JOYSTREAM_CORE_PAYOR_HPP
#define JOYSTREAM_CORE_PAYOR_HPP

#include <common/typesafeOutPoint.hpp>

#include <QObject>

namespace joystream {
namespace paymentchannel {
    class Payor;
}
namespace core {

class Payor : public QObject {

    Q_OBJECT

public:

    Payor(quint64 price,
          quint64 numberOfPaymentsMade,
          quint64 funds,
          quint64 settlementFee,
          quint32 refundLockTime,
          const Coin::typesafeOutPoint & anchor);

    static Payor * create(const paymentchannel::Payor &);

    // Size of single payment
    quint64 price() const noexcept;

    // Number of payments made
    quint64 numberOfPaymentsMade() const noexcept;

    // Funds allocated to output
    quint64 funds() const noexcept;

    // Settlement fee
    quint64 settlementFee() const noexcept;

    // Lock time of refund, received in
    quint32 refundLockTime() const noexcept;

    // Anchor for channel in contract transaction
    Coin::typesafeOutPoint anchor() const noexcept;

    /** Temporarily public ***/
    void update(const paymentchannel::Payor &);
    /** Temporarily public ***/

signals:

    // Size of single payment
    void priceChanged(quint64);

    // Number of payments made
    void numberOfPaymentsMadeChanged(quint64);

    // Funds allocated to output
    void fundsChanged(quint64);

    // Settlement fee
    void settlementFeeChanged(quint64);

    // Lock time of refund, received in
    void refundLockTimeChanged(quint32);

    // Anchor for channel in contract transaction
    void anchorChanged(const Coin::typesafeOutPoint &);

private:

    // Size of single payment
    quint64 _price;

    // Number of payments made
    quint64 _numberOfPaymentsMade;

    // Funds allocated to output
    quint64 _funds;

    // Settlement fee
    quint64 _settlementFee;

    // Lock time of refund, received in
    quint32 _refundLockTime;

    // Anchor for channel in contract transaction
    Coin::typesafeOutPoint _anchor;
};

}
}

#endif // JOYSTREAM_CORE_PAYOR_HPP
