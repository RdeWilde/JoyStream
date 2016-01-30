#include <blockcypher/TX.hpp>

#include <QDateTime>
#include <QDebug>

#include <CoinCore/CoinNodeData.h>

namespace BlockCypher {

    TX::TX(const QJsonObject & o) {
        _hash = o.value("hash").toString();

        _block_height = o.value("block_height").toInt();

        for(const QJsonValue & value : o.value("addresses").toArray()) {
            _addresses.append(value.toString());
        }

        _total = o.value("total").toInt();

        _fees = o.value("fees").toInt();

        _received = QDateTime::fromString(o.value("received").toString(),
                                          Qt::DateFormat::ISODate);
        _ver = o.value("ver").toInt();

        _lock_time = o.value("lock_time").toInt();

        _double_spend = o.value("double_spend").toBool();

        _vin_sz = o.value("vin_sz").toInt();

        _vout_sz = o.value("vout_sz").toInt();

        _confirmations = o.value("confirmations").toInt();

        for(const QJsonValue & value : o.value("inputs").toArray()) {
            _inputs.push_back(TXInput(value.toObject()));
        }

        for(const QJsonValue & value : o.value("outputs").toArray()) {
            _outputs.push_back(TXOutput(value.toObject()));
        }
    }

    Coin::Transaction TX::toTransaction() {
        Coin::Transaction tx;

        tx.version = _ver;
        tx.lockTime = _lock_time;

        for(const TXInput & input : _inputs) {
            tx.addInput(input.toInput());
        }

        for(const TXOutput & output : _outputs) {
            tx.addOutput(output.toOutput());
        }

        return tx;
    }
}
