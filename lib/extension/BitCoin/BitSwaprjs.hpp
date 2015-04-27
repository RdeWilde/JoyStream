#ifndef BITSWAPRJS_HPP
#define BITSWAPRJS_HPP

#include <QString>
#include <QVector>

#include "extension/PaymentChannel/Payor.hpp"

#include "extension/BitCoin/Wallet.hpp"

class QJsonValue;
class QJsonObject;
class KeyPair;
class Contract;
//class Hash;
class Refund;
//class PrivateKey;
class Signature;

class OutPoint;
//class Channel;
class P2PKHTxOut;
class P2SHTxOut;
class UnspentP2PKHOutput;

// Wraps BitSwaprjs
class BitSwaprjs
{
public:

    BitSwaprjs();

    BitSwaprjs(const QString & node, const QString & module);

    static QList<KeyPair> generate_fresh_key_pairs(int numberOfPairs);

    //static TxId compute_contract_hash(const OutPoint & fundingOutPoint, const quint64 fundingValue, const PrivateKey & sk, const QVector<P2SHTxOut> & contractOutputs, const P2PKHTxOut & changeOutput);

    static TxId compute_contract_hash(const UnspentP2PKHOutput & utxo, const QVector<P2SHTxOut> & contractOutputs, const P2PKHTxOut & changeOutput);

    /**
     * Signature computing/checking
     */
    static Signature compute_refund_signature(const OutPoint & contractOutputPoint, const PrivateKey &sk, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, quint32 refundLockTime);

    static Signature compute_payment_signature(const OutPoint & contractOutputPoint, const PrivateKey &sk, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, const P2PKHTxOut & paymentOutput);

    static bool check_refund_signatures(const OutPoint & contractOutputPoint, const Signature &payorSignature, const Signature &payeeSignature, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, quint32 refundLockTime);

    static bool check_payment_signatures(const OutPoint & contractOutputPoint, const Signature &payorSignature, const Signature &payeeSignature, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, const P2PKHTxOut & paymentOutput);


    /**
     * Wallet stuff
     */

    // We wont bother with an address class as that gets complicated with all the different address types.
    static QString to_address(const PublicKey & pk);

    static QMap<PublicKey, QList<Wallet::TxOEvent>> get_key_events(const QSet<PublicKey> & keys);

    static quint32 get_latest_block();

    /**
     * Broadcast transactions
     */
    static TxId broadcast_contract(const UnspentP2PKHOutput & utxo, const QVector<P2SHTxOut> & contractOutputs, const P2PKHTxOut & changeOutput);


    /**
     * Query block chain
     */

    static quint64 get_tx_outpoint(const OutPoint & point, bool & spent);

private:

    QString _node;

    QString _module;

    static QJsonValue nodeBlockingCall(const QString & method, const QJsonValue & params);
};

#endif // BITSWAPRJS_HPP
