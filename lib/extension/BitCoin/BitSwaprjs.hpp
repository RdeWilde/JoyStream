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

    //static Signature compute_contract_signature(const Contract & contract);

    //static Hash compute_contract_hash(const Contract & contract, const PrivateKey & sk);

    static Hash compute_contract_hash(const OutputPoint & fundingOutput, const PrivateKey & sk, const QVector<Payor::Channel> & channels, const P2PKHTxOut & changeOutput);

    static Signature compute_payor_refund_signature(const OutputPoint & contractOutputPoint, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, quint32 refundLockTime);






    static bool check_refund_signature(const Refund & refund, const PrivateKey & sk, const Contract & contract, const Signature & signature);

private:

    QString _node;

    QString _module;

    static QJsonObject nodeBlockingCall(const QString & method, const QJsonValue & params);
};

#endif // BITSWAPRJS_HPP
