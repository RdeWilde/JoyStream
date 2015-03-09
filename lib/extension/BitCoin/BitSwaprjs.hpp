#ifndef BITSWAPRJS_HPP
#define BITSWAPRJS_HPP

#include <QString>
#include <QVector>

#include "extension/PaymentChannel/Payor.hpp"

class QJsonValue;
class QJsonObject;
class KeyPair;
class Contract;
//class Hash;
class Refund;
//class PrivateKey;
class Signature;

class OutPoint;
class Channel;
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

    static Hash compute_contract_hash(const OutPoint & fundingOutput, const PrivateKey & sk, const QVector<Payor::Channel> & channels, const P2PKHTxOut & changeOutput);

    static Signature compute_payor_refund_signature(const OutPoint & contractOutputPoint, const PrivateKey &sk, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, quint32 refundLockTime);

    static bool check_refund_signatures(const OutPoint & contractOutputPoint, const Signature &payorSignature, const Signature &payeeSignature, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, quint32 refundLockTime);

    static Signature compute_payor_payment_signature(const OutPoint & contractOutputPoint, const P2PKHTxOut &refundOutput, const P2PKHTxOut & payeeOutput, const PrivateKey &sk);

    static UnspentP2PKHOutput get_utxo(quint64 minimalValue);

    // We wont bother with an address class as that gets complicated with all the different
    // address types.
    static QString to_address(const PublicKey & pk);

private:

    QString _node;

    QString _module;

    static QJsonValue nodeBlockingCall(const QString & method, const QJsonValue & params);
};

#endif // BITSWAPRJS_HPP
