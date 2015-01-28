#ifndef BITSWAPRJS_HPP
#define BITSWAPRJS_HPP

#include <QString>

class QJsonValue;
class QJsonObject;

class BitSwaprjs
{
public:
    BitSwaprjs(const QString & node, const QString & module);

    void test() const;

    void generate_fresh_key_pairs(int numberOfPairs) const;

private:

    QString _node;

    QString _module;

    QJsonObject nodeBlockingCall(const QString & method, const QJsonValue & params) const;
};

#endif // BITSWAPRJS_HPP
