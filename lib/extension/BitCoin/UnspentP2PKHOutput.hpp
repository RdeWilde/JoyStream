#ifndef UNSPENT_P2PKH_OUTPUT_HPP
#define UNSPENT_P2PKH_OUTPUT_HPP

#include "KeyPair.hpp"
#include "OutPoint.hpp"

class UnspentP2PKHOutput
{
public:

    // Default constructor
    UnspentP2PKHOutput();

    // Constructor from members
    UnspentP2PKHOutput(const KeyPair & _fundingOutputKeyPair, const OutPoint & fundingOutput, quint64 fundingValue);

    // Getters and setters
    KeyPair fundingOutputKeyPair() const;
    void setFundingOutputKeyPair(const KeyPair & fundingOutputKeyPair);

    OutPoint fundingOutput() const;
    void setFundingOutput(const OutPoint & fundingOutput);

    quint64 fundingValue() const;
    void setFundingValue(quint64 fundingValue);

private:

    // Controls utxo
    KeyPair _fundingOutputKeyPair;

    // TxId+index
    OutPoint _fundingOutput;

    // Value of output
    quint64 _fundingValue;

};

#endif // UNSPENT_P2PKH_OUTPUT_HPP
