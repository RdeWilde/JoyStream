/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 5 2015
 */

#ifndef OUPUT_FUNDS_PAYER_HPP
#define OUPUT_FUNDS_PAYER_HPP

#include <wallet/CoinWrappers.hpp>

#include <QByteArray>

class QSqlQuery;

// Many-to-many association entity
class OuputFundsPayer {

public:

    // Constructor from members
    OuputFundsPayer(quint64 _value, const QByteArray & pubKeyScript, quint64 _payerId);

    // Constructor from record
    // OuputFundsPayer(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery();

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery();

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery();

    // Getters and setters
    quint64 value() const;
    void setValue(quint64 value);

    QByteArray pubKeyScript() const;
    void setPubKeyScript(const QByteArray &pubKeyScript);

    quint64 payerId() const;
    void setPayerId(quint64 payerId);

private:

    /**
     * Output PK
     */

    // Number of satoshies in output
    quint64 _value;

    // Serialized output script
    QByteArray _pubKeyScript;

    // dont think we need an index

    // Payer to which output funds
    quint64 _payerId;
};

#endif // OUPUT_FUNDS_PAYER_HPP

