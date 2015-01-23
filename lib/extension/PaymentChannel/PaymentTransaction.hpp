#ifndef PAYMENT_TRANSACTION_HPP
#define PAYMENT_TRANSACTION_HPP

#include <QtGlobal> // quint32

#include "extension/BitCoin/TxOut.hpp"
#include "extension/BitCoin/PublicKey.hpp"

class QJsonObject;

class PaymentTransaction
{
public:

    // Constructor based on JSON of raw transaction
    PaymentTransaction(const QJsonObject & rawTransaction);

    // Constructor based on members
    PaymentTransaction(const OutputPoint & contractOutput,
                       quint64 payorFunds,
                       quint64 payeeFunds,
                       const PublicKey & payourKey,
                       const PublicKey & payeeKey);

    // To JSON with raw transaction
    QJsonObject rawTransaction() const;

    // Getters and setters
    OutputPoint contractOutput() const;
    void setContractOutput(const OutputPoint &contractOutput);

    quint64 payorFunds() const;
    void setPayorFunds(const quint64 &payorFunds);

    quint64 payeeFunds() const;
    void setPayeeFunds(const quint64 &payeeFunds);

    PublicKey payourKey() const;
    void setPayourKey(const PublicKey &payourKey);

    PublicKey payeeKey() const;
    void setPayeeKey(const PublicKey &payeeKey);

private:

    // Contract transaction output
    OutputPoint _contractOutput;

    // Funds going to payor
    quint64 _payorFunds;

    // Funds going to payee
    quint64 _payeeFunds;

    // Payor key
    PublicKey _payourKey;

    // Payee key
    PublicKey _payeeKey;
};

#endif // PAYMENT_TRANSACTION_HPP
