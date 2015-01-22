#ifndef REFUND_TANSACTION_HPP
#define REFUND_TANSACTION_HPP

#include <QtGlobal> // quint32

#include "extension/BitCoin/TxOut.hpp"

class QJsonObject;

class RefundTransaction
{
public:

    // Constructor based on JSON of raw transaction
    RefundTransaction(const QJsonObject & rawTransaction);

    // Constructor based on members
    RefundTransaction(const TxOut & contractOutput, quint64 funds);

    // To JSON with raw transaction
    QJsonObject rawTransaction() const;

    // Getters and setters
    TxOut contractOutput() const;
    void setContractOutput(const TxOut &contractOutput);

    quint64 funds() const;
    void setFunds(const quint64 &funds);

private:

    // Contract transaction output
    TxOut _contractOutput;

    // Funds going to payor
    quint64 _funds;
};

#endif // REFUND_TANSACTION_HPP

