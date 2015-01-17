#ifndef PAYOR_PAYMENT_CHANNEL_HPP
#define PAYOR_PAYMENT_CHANNEL_HPP

//#include<QSet>

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/PrivateKey.hpp"
#include "extension/BitCoin/Hash.hpp"
#include "extension/BitCoin/Signature.hpp"

/**
 * 1-to-N payment channel from payor perspective, using design in CBEP.
 * https://github.com/bedeho/CBEP
 *
 */
class PayorPaymentChannel
{
public:
    PayorPaymentChannel(quint32 numberOfPayees);

private:

    // Number of payees desired in the channel
    quint32 _numberOfPayees;

    /**
     * Represents payment channel participant as seen by a payor.
     */
    class Payee {

    public:

    private:


        //
        quint32 _outputIndex;

        //
        bool _waiting
    };

    // Vector of plugins for seller in contract,
    // where position corresponds to contract output position.
    QVector<T> _sellersInContract;


    QVector<PublicKey> _contractOutputPKs;

    /**
     * Contract
     */

    //PublicKey _sPK;
    //PrivateKey _sSK;

    // TxId of transactin funding contract
    // Hash _fundingTxHash;
    // quint32 _fundingTxOutputIndex;

    // TxId of contract transaction
    // Hash _contractTxHash
    // PublicKey _contractTxChangePK
    // PublicKey _contractTxO




};

#endif // PAYOR_PAYMENT_CHANNEL_HPP
