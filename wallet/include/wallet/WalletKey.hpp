/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef WALLET_KEY_HPP
#define WALLET_KEY_HPP

#include <QDateTime>

class WalletKey
{
public:

    enum class Purpose {
        Receive,
        ContractChange,
        PayerInContractOutput,
        ContractFinal,
        PayeeInContractOutput,
        ContractPayment
    };

    WalletKey();

    // Conversion routines
    static quint8 encodePurpose(Purpose purpose);
    static Purpose decodePurpose(quint8 encoded);

    // Getters and setters
    quint64 walletSequenceNumber() const;
    void setWalletSequenceNumber(quint64 walletSequenceNumber);

    Purpose purpose() const;
    void setPurpose(Purpose purpose);

    QDateTime generated() const;
    void setGenerated(const QDateTime & generated);

    QString description() const;
    void setDescription(const QString & description);

private:

    // The sequence number is wallet
    quint64 _walletSequenceNumber;

    // Private key
    // key

    // Purpose for which key was generated
    Purpose _purpose;

    // When key was generated
    QDateTime _generated;

    // Free form description
    QString _description;
};

#endif // WALLET_KEY_HPP
