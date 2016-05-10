/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 9 2015
 */

#include <blockcypher/TXRef.hpp>

#include <QJsonObject>
#include <QDebug>

namespace blockcypher {

TXRef::TXRef(const QJsonObject & o) {

    if(o.contains("address")){
        Q_ASSERT(o["address"].isString());
        _addressString = o["address"].toString().toStdString();
    }

    // is not included for unconfirmed txrefs
    if(o.contains("block_height")) {

        Q_ASSERT(o["block_height"].isDouble());
        _block_height = o["block_height"].toInt();
    }

    Q_ASSERT(o.contains("tx_hash"));
    Q_ASSERT(o["tx_hash"].isString());
    /** is this even correct ***/
    QString tx_hash = o["tx_hash"].toString();
    //qDebug() << o["tx_hash"].toString();
    _tx_hash = Coin::TransactionId::fromRPCByteOrder(tx_hash.toStdString());

    Q_ASSERT(o.contains("tx_input_n"));
    Q_ASSERT(o["tx_input_n"].isDouble());
    _tx_input_n = o["tx_input_n"].toInt();

    Q_ASSERT(o.contains("tx_output_n"));
    Q_ASSERT(o["tx_output_n"].isDouble());
    _tx_output_n = o["tx_output_n"].toInt();

    // At least one must be negative
    Q_ASSERT((_tx_input_n >= 0 && _tx_output_n < 0)
             ||
             (_tx_input_n < 0 && _tx_output_n >= 0));

    Q_ASSERT(o.contains("value"));
    Q_ASSERT(o["value"].isDouble());
    _value = o["value"].toInt();

    Q_ASSERT(o.contains("spent"));
    Q_ASSERT(o["spent"].isBool());
    _spent = o["spent"].toBool();

    Q_ASSERT(o.contains("double_spend"));
    Q_ASSERT(o["double_spend"].isBool());
    _double_spend = o["double_spend"].toBool();

    Q_ASSERT(o.contains("confirmations"));
    Q_ASSERT(o["confirmations"].isDouble());
    _confirmations = o["confirmations"].toInt();

    if(o.contains("script")){
        Q_ASSERT(o["script"].isString());
        _script = o["script"].toString().toStdString();
    }

    /**
    qDebug() << "o[tx_hash].toString():" << o["tx_hash"].toString();
    qDebug() << "QString tx_hash:" << tx_hash;
    qDebug() << "_tx_hash:" << QString::fromStdString(_tx_hash.toRPCByteOrder());

    if(_tx_hash.toRPCByteOrder() == "d7b74bdef4df85a61c9b89d9a82b0a5a20d21f0f5c1b50f6f02bd840c4585998")
        int x = 1;
    */
}

}
