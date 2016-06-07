/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 25 2015
 */

#include <blockcypher/TXOutput.hpp>
#include <blockcypher/ScriptType.hpp>

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <CoinCore/CoinNodeData.h>

namespace blockcypher {

    TXOutput::TXOutput(const QJsonObject & o) {

        if(!o.contains("value"))
            throw std::runtime_error("value field is not defined.");
        else if(!o.value("value").isDouble())
            throw std::runtime_error("value field is not a double.");
        else
            _value = o.value("value").toDouble();

        if(!o.contains("script"))
            throw std::runtime_error("script field is not defined.");
        else if(!o.value("script").isString())
            throw std::runtime_error("script field is not a string.");
        else
            _script = o.value("script").toString();

        if(!o.contains("addresses"))
            throw std::runtime_error("addresses field is not defined.");
        else if(!o.value("addresses").isArray())
            throw std::runtime_error("addresses field is not an array.");
        else {

            QJsonArray addresses = o.value("addresses").toArray();

            foreach (const QJsonValue & value, addresses) {

                if(!value.isString())
                    throw std::runtime_error("array contained non-string element.");
                else
                    _addresses.append(value.toString());
            }
        }

        if(!o.contains("script_type"))
            throw std::runtime_error("script_type is not defined.");
        else if(!o.value("script_type").isString())
            throw std::runtime_error("script_type is not a string.");
        else
            _script_type = toScriptType(o.value("script_type").toString());

        // optional if output is spent
        if(o.contains("spent_by") && o.value("spent_by").isString()) {
            _spent_by = o.value("spent_by").toString();
        }
    }

    bool TXOutput::operator==(const TXOutput & o) {

        return _value == o._value &&
               _script == o._script &&
               _addresses == o._addresses &&
               _script_type == o._script_type &&
               _spent_by == o._spent_by;
    }

    Coin::TxOut TXOutput::toOutput() const {
        return Coin::TxOut(_value, _script.toStdString());
    }
}
