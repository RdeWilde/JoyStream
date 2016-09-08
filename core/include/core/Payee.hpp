/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 17 2016
 */

#ifndef JOYSTREAM_CORE_PAYEE_HPP
#define JOYSTREAM_CORE_PAYEE_HPP

#include <common/typesafeOutPoint.hpp>

#include <QObject>

namespace joystream {
namespace paymentchannel {
    class Payee;
}
namespace core {

class Payee : public QObject {

    Q_OBJECT

public:

    /**
     * @brief Does MOC registration of all custome types used as signal arguments
     * on this and dependant QObjects.
     */
    static void registerMetaTypes();

    Payee(quint64 numberOfPaymentsMade,
          quint32 lockTime,
          quint64 price,
          quint64 funds,
          quint64 settlementFee,
          const Coin::typesafeOutPoint & anchor);

    static Payee * create(const paymentchannel::Payee &);

    // The number of payments which have been successfully made
    quint64 numberOfPaymentsMade() const noexcept;

    // Payment channel lock time
    quint32 lockTime() const noexcept;

    // Price (#satoshies) increment per payment
    quint64 price() const noexcept;

    // Amount (#satoshies) assigned to contract output
    quint64 funds() const noexcept;

    // Amount (#satoshies) used in fee for settlement
    quint64 settlementFee() const noexcept;

    // Contract outpoint from which payments originate
    Coin::typesafeOutPoint anchor() const noexcept;

    /** Temporarily public ***/
    void update(const paymentchannel::Payee &);
    /** Temporarily public ***/

signals:

    // Changed the number of payments which have been successfully made
    void numberOfPaymentsMadeChanged(quint64);

    // Cahnged payment channel lock time
    void lockTimeChanged(quint32);

    // Changed price (#satoshies) increment per payment
    void priceChanged(quint64);

    // Changed amount (#satoshies) assigned to contract output
    void fundsChanged(quint64);

    // Cahnged amount (#satoshies) used in fee for settlement
    void settlementFeeChanged(quint64);

    // Contract outpoint from which payments originate
    void anchorChanged(const Coin::typesafeOutPoint &);

private:

    // The number of payments which have been successfully made
    quint64 _numberOfPaymentsMade;

    // Payment channel lock time
    quint32 _lockTime;

    // Price (#satoshies) increment per payment
    quint64 _price;

    // Amount (#satoshies) assigned to contract output
    quint64 _funds;

    // Amount (#satoshies) used in fee for settlement
    quint64 _settlementFee;

    // Contract outpoint from which payments originate
    Coin::typesafeOutPoint _anchor;
};

}
}

#endif // JOYSTREAM_CORE_PAYEE_HPP
