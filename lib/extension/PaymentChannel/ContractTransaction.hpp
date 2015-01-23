#ifndef CONTRACT_TRANSACTION_HPP
#define CONTRACT_TRANSACTION_HPP

#include "extension/BitCoin/OutputPoint.hpp"
#include "extension/BitCoin/P2SHTxOut.hpp"
#include "extension/BitCoin/P2PKHTxOut.hpp"

class QJsonObject;

class ContractTransaction
{
public:
    /**
     * Default/Copy constructor and assignemtn operator needed to put in container.
     */
    ContractTransaction();
    ContractTransaction(const ContractTransaction& contractTransaction);
    ContractTransaction & operator=(const ContractTransaction& contractTransaction);

    // Constructor based on members
    ContractTransaction(quint32 numberOfMultisigOutputs);

    // Constructor based on JSON of raw transaction
    ContractTransaction(const QJsonObject & rawTransaction);

    // To JSON with raw transaction
    QJsonObject rawTransaction() const;

    // Getters and setters
    OutputPoint fundingOutput() const;
    void setFundingOutput(const OutputPoint &fundingOutput);

    QVector<P2SHTxOut> p2shTxOuts() const;
    void setP2shTxOuts(const QVector<P2SHTxOut> &p2shTxOuts);

    P2PKHTxOut refund() const;
    void setRefund(const P2PKHTxOut &refund);

private:

    // Unspent output funding channel
    OutputPoint _fundingOutput;

    // Multisig outputs
    QVector<P2SHTxOut> _p2shTxOuts;

    // Refund output
    P2PKHTxOut _refund;
};

#endif // CONTRACT_TRANSACTION_HPP
