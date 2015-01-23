#ifndef REFUND_TANSACTION_HPP
#define REFUND_TANSACTION_HPP

#include "extension/BitCoin/OutputPoint.hpp"
#include "extension/BitCoin/P2SHTxOut.hpp"

class QJsonObject;

class RefundTransaction
{
public:

    // Constructor based on JSON of raw transaction
    RefundTransaction(const QJsonObject & rawTransaction);

    // Constructor based on members
    RefundTransaction(const OutputPoint & contractOutput, const P2SHTxOut& refund, quint32 _lockTime);

    // To JSON with raw transaction
    QJsonObject rawTransaction() const;

    // Getters and setters
    OutputPoint contractOutput() const;
    void setContractOutput(const OutputPoint &contractOutput);

    P2SHTxOut refund() const;
    void setRefund(const P2SHTxOut &refund);

    quint32 lockTime() const;
    void setLockTime(const quint32 &lockTime);

private:

    // Contract transaction output
    OutputPoint _contractOutput;

    // Refund
    P2SHTxOut _refund;

    // How long is refund locked (UNIX time)
    quint32 _lockTime;
};

#endif // REFUND_TANSACTION_HPP
