/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#include <blockcypher/TXInput.hpp>
#include <CoinCore/CoinNodeData.h> // TxIn 
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


namespace BlockCypher {

TXInput::TXInput(const QJsonObject & o) {

        _prev_hash = o.value("prev_hash").toString();
        _output_index = o.value("output_index").toInt();
        _output_value = (quint64)o.value("output_value").toDouble();

        //Raw hexadecimal encoding of the script
        std::string t2 = (o.value("script").toString()).toUtf8().constData();
        _script = uchar_vector(t2);
        _script_type = toScriptType(o.value("script_type").toString());

        QJsonArray arr = o.value(("addresses")).toArray();

        foreach (const QJsonValue & value, arr) {
            QString t = value.toString();
            _addresses.append(t);
        }

        _sequence = (quint32)o.value("sequence").toDouble();

        //For debugging purposes.
        qDebug() << "\nTXInput object: " << o;
        qDebug() << "TXInput, Inserted _prev_hash:" << _prev_hash;
        qDebug() << "TXInput, Inserted _output_index:" << _output_index;
        qDebug() << "TXInput, Inserted _output_value:" << _output_value;
        qDebug() << "TXInput, Inserted _script:" << QString::fromUtf8(_script.getHex().c_str());
        qDebug() << "TXInput, Inserted _script_type:" << fromScriptType(_script_type);
        qDebug() << "TXInput, Inserted _addresses:" << _addresses;
        qDebug() << "TXInput, Inserted _sequence:" << _sequence;

}

bool TXInput::operator==(const TXInput & o) {
     return _prev_hash == o._prev_hash &&
            _output_index == o._output_index &&
            _output_value == o._output_value &&
            _script == o._script &&
            _script_type == o._script_type &&
            _addresses == o._addresses &&
            _sequence == o._sequence;
        // no need to check last one

}

Coin::TxIn TXInput::toInput() const {
    throw std::runtime_error("not done");
    return Coin::TxIn();
}

}
