#ifndef OUT_POINT_HPP
#define OUT_POINT_HPP

#include "extension/BitCoin/Hash.hpp"

class QJsonObject;

class OutPoint
{
public:

    /**
     * Default/Copy constructor and assignemtn operator needed to put in container.
     */
    OutPoint();
    OutPoint(const OutPoint& outputPoint);
    OutPoint & operator=(const OutPoint& outputPoint);

    // Constructor
    OutPoint(const Hash & hash, quint32 index);

    // Constructor from json
    OutPoint(const QJsonObject & json);

    // Comparison for use with QMap
    friend bool operator<(const OutPoint & lhs, const OutPoint & rhs);

    QJsonObject toJson() const;

    // Flat representation, so outpoint can be used as json key
    OutPoint(const QString & string);
    QString toString() const;

    // Getters and setters
    Hash hash() const;
    void setHash(const Hash &hash);

    quint32 index() const;
    void setIndex(const quint32 &index);

private:

    // TxId of transaction
    Hash _hash;

    // Output index
    quint32 _index;
};

#endif // OUT_POINT_HPP
