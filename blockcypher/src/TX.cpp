#include <blockcypher/TX.hpp>

#include <QDateTime>
#include <QDebug>

#include <CoinCore/CoinNodeData.h>
#include <common/BlockId.hpp>

namespace BlockCypher {

    TX::TX(const QJsonObject & o) {

        if(!o.contains("hash")){
            throw std::runtime_error("hash is not a key.");
        } else {
            if(!o.value("hash").isString())
                throw std::runtime_error("hash is not a string");

            _hash = o.value("hash").toString();
        }

        if(!o.contains("block_height")){
            throw std::runtime_error("block_height is not a key.");
        } else {
            if(!o.value("block_height").isDouble())
                throw std::runtime_error("block_height is not a double.");

            _block_height = o.value("block_height").toDouble();
        }

        if(!o.contains("addresses")){
            throw std::runtime_error("addresses is not a key.");
        } else {
            if(!o.value("addresses").isArray())
            throw std::runtime_error("addresses is not an array.");

            for(const QJsonValue & value : o.value("addresses").toArray()) {
                _addresses.append(value.toString());
            }
        }

        if(!o.contains("total")){
            throw std::runtime_error("total is not a key.");
        } else {
            if(!o.value("total").isDouble())
            throw std::runtime_error("total is not a double.");

            _total = o.value("total").toDouble();
        }

        if(!o.contains("fees") ){
            throw std::runtime_error("fees is not a key.");
        } else {
            if(!o.value("fees").isDouble())
            throw std::runtime_error("fees is not a double.");

            _fees = o.value("fees").toDouble();
        }

        if(!o.contains("size") ){
            throw std::runtime_error("size is not a key.");
        } else {
            if(!o.value("size").isDouble())
            throw std::runtime_error("size is not a double.");

            _size = o.value("size").toDouble();
        }

        if(!o.contains("received")){
            throw std::runtime_error("received is not a key.");
        } else {
            if(!o.value("received").isString())
            throw std::runtime_error("received is not a string.");

            _received = QDateTime::fromString(o.value("received").toString(),
                                              Qt::DateFormat::ISODate);
        }

        if(!o.contains("ver")){
            throw std::runtime_error("ver is not a key.");
        } else {
            if(!o.value("ver").isDouble())
            throw std::runtime_error("ver is not a double.");

            _ver = o.value("ver").toDouble();
        }

        if(!o.contains("lock_time")){
            throw std::runtime_error("lock_time is not a key.");
        } else {
            if(!o.value("lock_time").isDouble())
            throw std::runtime_error("lock_time is not a double.");

            _lock_time = o.value("lock_time").toDouble();
        }

        if(!o.contains("double_spend")){
            throw std::runtime_error("double_spend is not a key.");
        } else {
            if(!o.value("double_spend").isBool())
            throw std::runtime_error("double_spend is not a bool.");

            _double_spend = o.value("double_spend").toBool();
        }

        if(!o.contains("vin_sz")){
            throw std::runtime_error("vin_sz is not a key.");
        } else {
            if(!o.value("vin_sz").isDouble())
            throw std::runtime_error("vin_sz is not a double.");

            _vin_sz = o.value("vin_sz").toDouble();
        }

        if(!o.contains("vout_sz")){
            throw std::runtime_error("vout_sz is not a key.");
        } else {
            if(!o.value("vout_sz").isDouble())
            throw std::runtime_error("vout_sz is not a double.");

            _vout_sz = o.value("vout_sz").toDouble();
        }

        if(!o.contains("confirmations")){
            throw std::runtime_error("confirmations is not a key.");
        } else {
            if(!o.value("confirmations").isDouble())
            throw std::runtime_error("confirmations is not a double.");

            _confirmations = o.value("confirmations").toDouble();
        }

        if(!o.contains("inputs")){
            throw std::runtime_error("inputs is not a key.");
        } else {
            if(!o.value("inputs").isArray())
            throw std::runtime_error("inputs is not an array.");

            for(const QJsonValue & value : o.value("inputs").toArray()) {
                _inputs.push_back(TXInput(value.toObject()));
            }
        }

        if(!o.contains("outputs")){
            throw std::runtime_error("outputs is not a key.");
        } else {
            if(!o.value("outputs").isArray())
            throw std::runtime_error("outputs is not an array.");

            for(const QJsonValue & value : o.value("outputs").toArray()) {
                _outputs.push_back(TXOutput(value.toObject()));
            }
        }

        //optional values

        if(o.contains("confirmed") && o.value("confirmed").isString()) {
            _confirmed = QDateTime::fromString(o.value("confirmed").toString(),
                                               Qt::DateFormat::ISODate);
        }

        if(o.contains("block_hash") && o.value("block_hash").isString()) {
            _block_hash = Coin::BlockId(o.value("block_hash").toString());
        }

        if(o.contains("double_of") && o.value("double_of").isString()) {
            _double_of = o.value("double_of").toString();
        }

        if(o.contains("double_spend_tx") && o.value("double_spend_tx").isString()) {
            _double_spend_tx = o.value("double_spend_tx").toString();
        }

        if(o.contains("confidence") && o.value("confidence").isDouble()) {
            _confidence = o.value("confidence").toDouble();
        }

        if(o.contains("next_inputs") && o.value("next_inputs").isString()) {
            _next_inputs = o.value("next_inputs").toString();
        }

        if(o.contains("next_outputs") && o.value("next_outputs").isString()) {
            _next_outputs = o.value("next_outputs").toString();
        }
    }

    Coin::Transaction TX::toTransaction() const {

        if(_vin_sz > _inputs.size() || _vout_sz > _outputs.size()) {
            throw std::runtime_error("remaining inputs/outputs for this tx need to be fetched");
        }

        Coin::Transaction tx;

        tx.version = _ver;
        tx.lockTime = _lock_time;

        for(const TXInput & input : _inputs) {
            tx.addInput(input.toInput());
        }

        for(const TXOutput & output : _outputs) {
            tx.addOutput(output.toOutput());
        }

        Q_ASSERT(_hash.toStdString() == tx.hash().getHex());
        return tx;
    }
}
