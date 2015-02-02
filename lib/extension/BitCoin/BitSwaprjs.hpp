#ifndef BITSWAPRJS_HPP
#define BITSWAPRJS_HPP

#include <QString>

class QJsonValue;
class QJsonObject;
class KeyPair;
class Contract;
class Hash;
class Refund;
class PrivateKey;
class Signature;

class BitSwaprjs
{
public:
    BitSwaprjs(const QString & node, const QString & module);

    void test() const;

    static QList<KeyPair> generate_fresh_key_pairs(int numberOfPairs);

    static Hash compute_contract_hash(const Contract & contract);

    static bool check_refund_signature(const Refund & refund, const PrivateKey & sk, const Contract & contract, const Signature & signature);

private:

    QString _node;

    QString _module;

    static QJsonObject nodeBlockingCall(const QString & method, const QJsonValue & params);
};

#endif // BITSWAPRJS_HPP
