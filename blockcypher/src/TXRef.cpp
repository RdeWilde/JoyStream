/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 9 2015
 */

#include <blockcypher/TXRef.hpp>

#include <QJsonObject>
#include <QDebug>

namespace BlockCypher {

TXRef::TXRef(const QJsonObject & o) {

    Q_ASSERT(o.contains("address"));
    Q_ASSERT(o["address"].isString());
    _addressString = o["address"].toString().toStdString();

    Q_ASSERT(o.contains("block_height"));
    Q_ASSERT(o["block_height"].isDouble());
    _block_height = o["block_height"].toInt();

    Q_ASSERT(o.contains("tx_hash"));
    Q_ASSERT(o["tx_hash"].isString());
    /** is this even correct ***/
    QString tx_hash = o["tx_hash"].toString();
    _tx_hash = Coin::TransactionId::fromLittleEndianHex(tx_hash.toStdString());

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
}

}
