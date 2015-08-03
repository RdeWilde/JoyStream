#ifndef INPUT_HPP
#define INPUT_HPP

#include <wallet/CoinWrappers.hpp>
#include <wallet/OutPoint.hpp>

#include <QByteArray>

class Input {

public:

    // Constructor for members
    Input(const OutPoint & outPoint, const QByteArray & scriptSig, quint32 sequenc);

    // Constructor from record
    // Input(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery();

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery();

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery();

    // Getters and setters
    OutPoint outPoint() const;
    void setOutPoint(const OutPoint & outPoint);

    QByteArray scriptSig() const;
    void setScriptSig(const QByteArray & scriptSig);

    quint32 sequence() const;
    void setSequence(quint32 sequence);

private:

    // OutPoint being spent by input
    OutPoint _outPoint;

    // Serialized Input script
    QByteArray _scriptSig;

    // Input sequence number
    quint32 _sequence;

};

#endif // INPUT_HPP
