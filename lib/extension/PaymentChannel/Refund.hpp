#ifndef REFUND_HPP
#define REFUND_HPP

#include "extension/BitCoin/OutputPoint.hpp"
#include "extension/BitCoin/P2PKHTxOut.hpp"

class QJsonObject;
class Signature;

class Refund
{
public:

    // Constructor based on JSON of raw transaction
    Refund(const QJsonObject & rawTransaction);

    // Constructor based on members
    Refund(const OutputPoint & contractOutput, const P2PKHTxOut & ouput, quint32 lockTime);

    // To JSON with raw transaction
    QJsonObject rawTransaction() const;

    // Checks if signature is valid for refund in index'th slot
    bool isRefundValid(const Signature &payorSignature, const Signature &payeeSignature) const;

    // Getters and setters
    OutputPoint contractOutput() const;
    void setContractOutput(const OutputPoint &contractOutput);

    P2PKHTxOut output() const;
    void setOutput(const P2PKHTxOut &output);

    quint32 lockTime() const;
    void setLockTime(const quint32 &lockTime);

private:

    // Contract transaction output
    OutputPoint _contractOutput;

    // Refund output
    P2PKHTxOut _output;

    // How long is refund locked (UNIX time)
    quint32 _lockTime;
};

#endif // REFUND_HPP
