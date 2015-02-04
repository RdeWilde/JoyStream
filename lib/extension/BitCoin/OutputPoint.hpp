#ifndef OUTPUT_POINT_HPP
#define OUTPUT_POINT_HPP

#include "extension/BitCoin/Hash.hpp"

class QJsonObject;

class OutputPoint
{
public:

    /**
     * Default/Copy constructor and assignemtn operator needed to put in container.
     */
    OutputPoint();
    OutputPoint(const OutputPoint& outputPoint);
    OutputPoint & operator=(const OutputPoint& outputPoint);

    // Constructor
    OutputPoint(const Hash & hash, quint32 index);

    QJsonObject json() const;

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

#endif // OUTPUT_POINT_HPP
