#ifndef CONTRACT_HPP
#define CONTRACT_HPP

#include "extension/BitCoin/OutPoint.hpp"
//#include "extension/BitCoin/Signature.hpp"
#include "extension/BitCoin/P2SHTxOut.hpp"
#include "extension/BitCoin/P2PKHTxOut.hpp"

class QJsonObject;
//class PrivateKey;
//class Signature;

#include <QVector>

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
    Contract(const OutPoint &fundingOutput, quint32 numberOfMultisigOutputs, const P2PKHTxOut &change);

    // Constructor based on bitswaprjsEncoding
    //void sign(const PrivateKey & sk);

    // To JSON encoding
    QJsonObject json() const;

    // Getters and setters
    OutPoint fundingOutput() const;
    void setFundingOutput(const OutPoint &fundingOutput);

    QVector<P2SHTxOut> p2shTxOuts() const;
    void setP2shTxOuts(const QVector<P2SHTxOut> &p2shTxOuts);

    P2PKHTxOut change() const;
    void setChange(const P2PKHTxOut &change);

    // Set corresponding output with given value
    void setOutput(quint32 index, const P2SHTxOut& output);

private:

    // Unspent output funding channel
    OutPoint _fundingOutput;

    // Multisig outputs
    QVector<P2SHTxOut> _p2shTxOuts;

    // Refund output
    P2PKHTxOut _change;
};

#endif // CONTRACT_HPP
