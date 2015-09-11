/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#include <blockcypher/Address.hpp>
#include <blockcypher/TXRef.hpp>
#include <QJsonObject>
#include <QJsonArray>

namespace BlockCypher {

Address::Address() {
}

Address::Address(const QJsonObject & o) {

    Q_ASSERT(o.contains("total_received"));
    Q_ASSERT(o["total_received"].isDouble());
    _total_received = o["total_received"].toInt();

    Q_ASSERT(o.contains("total_sent"));
    Q_ASSERT(o["total_sent"].isDouble());
    _total_sent = o["total_sent"].toInt();

    Q_ASSERT(o.contains("balance"));
    Q_ASSERT(o["balance"].isDouble());
    _balance = o["balance"].toInt();

    Q_ASSERT(o.contains("unconfirmed_balance"));
    Q_ASSERT(o["unconfirmed_balance"].isDouble());
    _unconfirmed_balance = o["unconfirmed_balance"].toInt();

    Q_ASSERT(o.contains("final_balance"));
    Q_ASSERT(o["final_balance"].isDouble());
    _final_balance = o["final_balance"].toInt();

    Q_ASSERT(o.contains("txrefs"));
    Q_ASSERT(o["txrefs"].isArray());
    QJsonArray txrefs = o["txrefs"].toArray();

    for(QJsonArray::const_iterator i = txrefs.constBegin(),
        end = txrefs.constEnd();
        i != end;
        i++) {

        QJsonValue elm = *i;

        _txrefs.push_back(TXRef(elm.toObject()));
    }
}

}
