/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#include <blockcypher/TXInput.hpp>
#include <blockcypher/ScriptType.hpp>

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include <CoinCore/CoinNodeData.h>
#include <stdutils/uchar_vector.h>

namespace BlockCypher {

    TXInput::TXInput(const QJsonObject & o) {

        if(!o.contains("prev_hash"))
            throw std::runtime_error("prev_hash is not a key.");
        else {

            QJsonValue prev_hash = o.value("prev_hash");

            if(!prev_hash.isString())
                throw std::runtime_error("prev_hash is not a string.");
            else
                _prev_hash = prev_hash.toString();

        }

        if(!o.contains("output_index"))
            throw std::runtime_error("output_index is not a key.");
        else {

            QJsonValue output_index = o.value("output_index");

            if(!output_index.isDouble())
                throw std::runtime_error("output_index is not a double.");
            else
                _output_index = o.value("output_index").toInt();
        }

        if(!o.contains("output_value"))
            throw std::runtime_error("output_value is not a key.");
        else {

            QJsonValue output_value = o.value("output_value");

            if(!output_value.isDouble())
                throw std::runtime_error("output_value is not a double.");
            else
                _output_value = output_value.toInt();
        }

        if(!o.contains("script"))
            throw std::runtime_error("script is not a key.");
        else {

            QJsonValue script = o.value("script");

            if(!script.isString())
                throw std::runtime_error("script is not a string.");
            else
                _script = script.toString();
        }


        if(!o.contains("script_type"))
            throw std::runtime_error("script_type is not a key.");
        else {

            QJsonValue script_type = o.value("script_type");

            if(!script_type.isString())
                throw std::runtime_error("script_type is not a string.");
            else
                _script_type = toScriptType(script_type.toString());
        }

        if(!o.contains("addresses"))
            throw std::runtime_error("addresses key is not defined.");
        else {

            QJsonValue addresses = o.value("addresses");

            if(!addresses.isArray())
                throw std::runtime_error("addresses is not an array.");
            else {

                QJsonArray addressesArray = addresses.toArray();

                foreach (const QJsonValue & value, addressesArray) {

                    if(!value.isString())
                        throw std::runtime_error("array contained non-string element.");
                    else
                        _addresses.append(value.toString());
                }

            }
        }

        if(!o.contains("sequence"))
            throw std::runtime_error("sequence key is not defined.");
        else {

            QJsonValue sequence = o.value("sequence");

            if(!sequence.isDouble())
                throw std::runtime_error("sequence is not a double.");
            else{
                _sequence = sequence.toInt();
            }
        }
    }

    bool TXInput::operator==(const TXInput & o) {

        return _prev_hash == o._prev_hash &&
               _output_index == o._output_index &&
               _output_value == o._output_value &&
               _script == o._script &&
               _script_type == o._script_type &&
               _addresses == o._addresses &&
               _sequence == o._sequence;
    }

    Coin::TxIn TXInput::toInput() const {
        uchar_vector op(_prev_hash.toStdString());
        //op.reverse();
        Coin::OutPoint outpoint(op, _output_index);
        return Coin::TxIn(outpoint, _script.toStdString(), _sequence);
    }
}
