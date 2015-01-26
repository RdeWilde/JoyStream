#ifndef CONTRACT_HPP
#define CONTRACT_HPP

#include "extension/BitCoin/OutputPoint.hpp"
#include "extension/BitCoin/P2SHTxOut.hpp"
#include "extension/BitCoin/P2PKHTxOut.hpp"

class QJsonObject;

class Contract
{
public:
    /**
     * Default/Copy constructor and assignemtn operator needed to put in container.
     */
    Contract();
    Contract(const Contract& contract);
    Contract & operator=(const Contract& contract);

    // Constructor based on members
    Contract(quint32 numberOfMultisigOutputs);

    // Constructor based on JSON of raw transaction
    Contract(const QJsonObject & rawTransaction);

    // To JSON with raw transaction
    QJsonObject rawTransaction() const;

    // Getters and setters
    OutputPoint fundingOutput() const;
    void setFundingOutput(const OutputPoint &fundingOutput);

    QVector<P2SHTxOut> p2shTxOuts() const;
    void setP2shTxOuts(const QVector<P2SHTxOut> &p2shTxOuts);

    P2PKHTxOut change() const;
    void setChange(const P2PKHTxOut &change);

    // Set corresponding output with given value
    void setOutput(quint32 index, const P2SHTxOut& output);

private:

    // Unspent output funding channel
    OutputPoint _fundingOutput;

    // Multisig outputs
    QVector<P2SHTxOut> _p2shTxOuts;

    // Refund output
    P2PKHTxOut _change;
};

#endif // CONTRACT_HPP
