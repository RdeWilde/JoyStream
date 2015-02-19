#ifndef REFUND_HPP
#define REFUND_HPP

#include "extension/BitCoin/OutPoint.hpp"
#include "extension/BitCoin/P2PKHTxOut.hpp"

class QJsonObject;
class Signature;

class Refund
{
public:

    // Constructor based on JSON with encoding
    Refund(const QJsonObject & json);

    // Constructor based on members
    Refund(const OutPoint & contractOutput, const P2PKHTxOut & ouput, quint32 lockTime);

    // To JSON with encoding
    QJsonObject json() const;

    // Checks if signature is valid for refund in index'th slot
    bool isRefundValid(const Signature &payorSignature, const Signature &payeeSignature) const;

    // Getters and setters
    OutPoint contractOutput() const;
    void setContractOutput(const OutPoint &contractOutput);

    P2PKHTxOut output() const;
    void setOutput(const P2PKHTxOut &output);

    quint32 lockTime() const;
    void setLockTime(const quint32 &lockTime);

private:

    // Contract transaction output
    OutPoint _contractOutput;

    // Refund output
    P2PKHTxOut _output;

    // How long is refund locked (UNIX time)
    quint32 _lockTime;
};

#endif // REFUND_HPP
