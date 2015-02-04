#ifndef BITSWAPRJS_HPP
#define BITSWAPRJS_HPP

#include <QString>
#include <QVector>

#include "extension/PaymentChannel/Payor.hpp"

class QJsonValue;
class QJsonObject;
class KeyPair;
class Contract;
class Hash;
class Refund;
class PrivateKey;
class Signature;

class OutputPoint;
class P2PKHTxOut;
class P2SHTxOut;

class BitSwaprjs
{
public:
    BitSwaprjs(const QString & node, const QString & module);

    static QList<KeyPair> generate_fresh_key_pairs(int numberOfPairs);

    static Hash compute_contract_hash(const OutputPoint & fundingOutput, const PrivateKey & sk, const QVector<Payor::Channel> & channels, const P2PKHTxOut & changeOutput);

    static Signature compute_payor_refund_signature(const OutputPoint & contractOutputPoint, const PrivateKey &sk, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, quint32 refundLockTime);

    static bool check_refund_signatures(const OutputPoint & contractOutputPoint, const Signature &payorSignature, const Signature &payeeSignature, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, quint32 refundLockTime);

private:

    QString _node;

    QString _module;

    static QJsonObject nodeBlockingCall(const QString & method, const QJsonValue & params);
};

#endif // BITSWAPRJS_HPP
