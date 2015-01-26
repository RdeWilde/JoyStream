#ifndef PAYMENT_HPP
#define PAYMENT_HPP

#include "extension/BitCoin/OutputPoint.hpp"
#include "extension/BitCoin/P2PKHTxOut.hpp"

class QJsonObject;

class Payment
{
public:

    // Constructor based on JSON of raw transaction
    Payment(const QJsonObject& rawTransaction);

    // Constructor based on members
    Payment(const OutputPoint& contractOutput,
            const P2PKHTxOut& payorOutput,
            const P2PKHTxOut& payeeOutput);

    // To JSON with raw transaction
    QJsonObject rawTransaction() const;

    // Getters and setters
    OutputPoint contractOutput() const;
    void setContractOutput(const OutputPoint &contractOutput);

    P2PKHTxOut payorOutput() const;
    void setPayorOutput(const P2PKHTxOut &payorOutput);

    P2PKHTxOut payeeOutput() const;
    void setPayeeOutput(const P2PKHTxOut &payeeOutput);

private:

    // Contract transaction output
    OutputPoint _contractOutput;

    // Payor refund output
    P2PKHTxOut _payorOutput;

    // Payee payment output
    P2PKHTxOut _payeeOutput;
};

#endif // PAYMENT_HPP
