

#include "TXOutput.hpp"
#include "../../../shared_folder/JoyStream/deps/linux/src/mSIGNA/sysroot/include/CoinCore/CoinNodeData.h" // TxIn
#include "ScriptType.hpp"
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

namespace BlockCypher {

//Populate TXOutput from QJsonObject
TXOutput::TXOutput(const QJsonObject & o) {

    qDebug() << "\nTXOutput object: " << o;

    _value = (qint64)o.value("value").toDouble();

    //TODO: Raw hexadecimal encoding of the script
    std::string str = (o.value("script").toString()).toUtf8().constData();
    _script = uchar_vector(str);

    QJsonArray arr = o.value(("addresses")).toArray();

    foreach (const QJsonValue & value, arr) {
        QString t = value.toString();
        _addresses.append(t);
    }
        _script_type = toScriptType(o.value("script_type").toString());

        //For debugging purposes.
        qDebug() << "TXOutput, Inserted _value:" << _value;
        qDebug() << "TXOutput, Inserted _script:" << QString::fromUtf8(_script.getHex().c_str());
        qDebug() << "TXOutput, Inserted _addresses:" << _addresses;
        qDebug() << "TXOutput, Inserted _script_type:" << fromScriptType(_script_type);

}

bool TXOutput::operator==(const TXOutput & o) {
   // return value == o.value &&
   //         script == o.script;
    // no need to check last one
    throw std::runtime_error("not done");
}

//Coin::TxIn TXOutput::toInput() const {
//    throw std::runtime_error("not done");
//    return Coin::TxIn();
//}

} // end namespace BlockCypher
