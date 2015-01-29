#ifndef BITSWAPRJS_HPP
#define BITSWAPRJS_HPP

#include <QString>

class QJsonValue;
class QJsonObject;
class KeyPair;
class Contract;
class Hash;

class BitSwaprjs
{
public:
    BitSwaprjs(const QString & node, const QString & module);

    void test() const;

    static QList<KeyPair> generate_fresh_key_pairs(int numberOfPairs);

    static Hash compute_contract_hash(const Contract & contract);

private:

    QString _node;

    QString _module;

    static QJsonObject nodeBlockingCall(const QString & method, const QJsonValue & params);
};

#endif // BITSWAPRJS_HPP
