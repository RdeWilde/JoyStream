#include "BitSwaprjs.hpp"
#include "KeyPair.hpp"
#include "P2PKHTxOut.hpp"
#include "UnspentP2PKHOutput.hpp"

//#include "extension/PaymentChannel/Contract.hpp"
//#include "extension/PaymentChannel/Refund.hpp"

#include "extension/PaymentChannel/Payor.hpp"

#include "Signature.hpp"

#include <QProcess>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QDebug>

BitSwaprjs::BitSwaprjs()
    : _node("C:\\Program Files\\nodejs\\node.exe")
    , _module("C:\\Users\\Sindre\\Documents\\GitHub\\BitSwaprjs\\index.js") {
}

BitSwaprjs::BitSwaprjs(const QString & node, const QString & module)
    : _node(node)
    , _module(module) {
}

QJsonValue BitSwaprjs::nodeBlockingCall(const QString & method, const QJsonValue & params) {

    // Build input dictionary for node script
    QJsonObject input;
    input["method"] = method;
    input["params"] = params;

    // Turn into string form
    QJsonDocument inputDocument(input);
    QByteArray byteArrayInput(inputDocument.toJson(QJsonDocument::Compact));
    QString encodedInputs = QString(byteArrayInput).replace("\"","#");

    // qDebug() << encodedInputs;

    // Build argument list
    QStringList args;
    args << "C:/Users/Sindre/Documents/GitHub/BitSwaprjs/index.js" << encodedInputs;

    // Allocate process and loop
    QProcess process;
    QEventLoop loop;

    // Stop event loop when processing is done
    QObject::connect(&process,
                     SIGNAL(finished(int, QProcess::ExitStatus)),
                     &loop,
                     SLOT(quit()));

    // Start process async
    process.start("C:/Program Files/nodejs/node.exe", args);

    // Wait for process to be finished
    loop.exec();

    // Read what process dumped
    QByteArray rawOutput = process.readAll();

    //qDebug() << rawOutput;

    // Decode into json
    QJsonParseError parseError;
    QJsonDocument outputDocument = QJsonDocument::fromJson(rawOutput, &parseError);

    Q_ASSERT(parseError.error == QJsonParseError::NoError);

    // Return json object
    QJsonObject o = outputDocument.object();

    // get value for state key2
    QJsonValue successValue = o["success"];

    Q_ASSERT(successValue.type() == QJsonValue::Bool);

    if(!successValue.toBool())
        throw std::exception("Error state returned.");
    else
        return o["result"];
}

QList<KeyPair> BitSwaprjs::generate_fresh_key_pairs(int numberOfPairs) {

    // Make call to generate keys
    QJsonValue result = nodeBlockingCall("generate_fresh_key_pairs", QJsonValue(numberOfPairs));

    Q_ASSERT(result.type() == QJsonValue::Array);

    QJsonArray jsonKeyPairsArray = result.toArray();

    // Create list for key pairs
    QList<KeyPair> keyPairs;

    for(QJsonArray::const_iterator i = jsonKeyPairsArray.constBegin();
        i != jsonKeyPairsArray.constEnd();i++) {

        // Get json array element
        QJsonValue element = *i;

        // Turn into map
        QJsonObject map = element.toObject();

        // Get keypair from map and add to keypair list
        QString pkString = map["pk"].toString();
        QString skString = map["sk"].toString();

        QString addressString = map["address"].toString();

        keyPairs.append(KeyPair(PublicKey(pkString), PrivateKey(skString)));
    }

    return keyPairs;
}

/**
Hash BitSwaprjs::compute_contract_hash(const Contract & contract, const PrivateKey & sk) {

    // Create json parameters
    QJsonObject encodedContract = contract.json();

    QJsonObject params {
      {"contract", encodedContract},
      {"sk", sk.toString()}
    };

    // Make call
    QJsonValue result = nodeBlockingCall("compute_contract_hash", QJsonValue(params));

    // Turn string to hash
    return Hash(result.toString());
}
*/

TxId BitSwaprjs::compute_contract_hash(const OutPoint & fundingOutPoint, const PrivateKey & sk, const QVector<Payor::Channel> & channels, const P2PKHTxOut & changeOutput) {

    // Encode parameters into json
    QJsonArray p2shTxOuts;

    for(QVector<Payor::Channel>::const_iterator i = channels.begin(), end(channels.end()); i != end;i++)
        p2shTxOuts.append(i->json());

    QJsonObject params {
        {"fundingOutput", fundingOutPoint.toJson()},
        {"p2shTxOuts", p2shTxOuts},
        {"change", changeOutput.json()},
        {"sk", sk.toString()}
    };

    // Make call
    QJsonValue result = nodeBlockingCall("compute_contract_hash", QJsonValue(params));

    // Turn string to hash
    return TxId(result.toString());
}

Signature BitSwaprjs::compute_payor_refund_signature(const OutPoint & contractOutputPoint, const PrivateKey &sk, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, quint32 refundLockTime) {

    // Create parameters
    QJsonObject params {
        {"contractOutputPoint", contractOutputPoint.toJson()},
        {"sk", sk.toString()},
        {"firstPk", firstPk.toString()},
        {"secondPk", secondPk.toString()},
        {"refundOutput", refundOutput.json()},
        {"refundLockTime", static_cast<qint64>(refundLockTime)}
    };

    // Make call
    QJsonValue result = nodeBlockingCall("compute_payor_refund_signature", QJsonValue(params));

    // Turn string to signature
    return Signature(result.toString());
}

bool BitSwaprjs::check_refund_signatures(const OutPoint & contractOutputPoint, const Signature & payorSignature, const Signature & payeeSignature, const PublicKey & firstPk, const PublicKey & secondPk, const P2PKHTxOut & refundOutput, quint32 refundLockTime) {

    // Create parameters
    QJsonObject params {
        {"contractOutputPoint", contractOutputPoint.toJson()},
        {"payorSignature", payorSignature.toString()},
        {"payeeSignature", payeeSignature.toString()},
        {"firstPk", firstPk.toString()},
        {"secondPk", secondPk.toString()},
        {"refundOutput", refundOutput.json()},
        {"refundLockTime", static_cast<qint64>(refundLockTime)}
    };

    // Make call
    QJsonValue result = nodeBlockingCall("check_refund_signatures", QJsonValue(params));

    // Turn into bool result
    return result.toBool();
}

QString BitSwaprjs::to_address(const PublicKey & pk) {

    // Create parameters
    QJsonObject params {
        {"pk", pk.toString()},
    };

    // Make call
    QJsonValue result = nodeBlockingCall("to_address", QJsonValue(params));

    // Turn into bool result
    return result.toString();
}

//QList<Wallet::TxOEvent> BitSwaprjs::get_key_events(const QList<PublicKey> & list) {
QMap<PublicKey, QList<Wallet::TxOEvent>> BitSwaprjs::get_key_events(const QSet<PublicKey> & keys) {

    QJsonArray pks;

    for(QSet<PublicKey>::const_iterator i = keys.constBegin();
        i != keys.constEnd();i++)
        pks.append((*i).toString());

    // Create parameters
    QJsonObject params {
        {"pks", pks},
    };

    // Make call
    QJsonValue result = nodeBlockingCall("get_key_events", QJsonValue(params));

    // Parse
    Q_ASSERT(result.type() == QJsonValue::Object);
    QJsonObject resultMap = result.toObject();

    QMap<PublicKey, QList<Wallet::TxOEvent>> map;

    for(QJsonObject::const_iterator i = resultMap.constBegin();
        i != resultMap.constEnd();i++) {

        QList<Wallet::TxOEvent> txOEvents;

        QJsonValue & eventList = i.value();
        Q_ASSERT(eventList.type() == QJsonValue::Array);
        QJsonArray resultArray = eventList.toArray();

        for(QJsonArray::const_iterator j = resultArray.constBegin();
            j != resultArray.constEnd();j++) {

            // Grab element
            QJsonValue & element = *j;

            Q_ASSERT(element.type() == QJsonValue::Object);

            // Turn into QJsonObject
            QJsonObject o = element.toObject();

            // Parse
            Wallet::TxOEvent event(o);

            // Add to events array
            txOEvents.append(event);
        }

        PublicKey key(i.key());
        map[key] = txOEvents;
    }

    return map;
}

quint32 BitSwaprjs::get_latest_block() {

    // Make call
    QJsonValue result = nodeBlockingCall("get_latest_block", QJsonValue(QJsonObject()));

    // Parse
    Q_ASSERT(result.type() == QJsonValue::Object);

    QJsonObject & blockObject = result.toObject();

    QJsonValue heightValue = blockObject["height"];

    Q_ASSERT(heightValue.type() == QJsonValue::Double);

    return heightValue.toDouble();
}

void BitSwaprjs::broadcast_contract(const OutPoint & fundingOutPoint, const PrivateKey & sk, const QVector<Payor::Channel> & channels, const P2PKHTxOut & changeOutput) {

    // Encode parameters into json
    QJsonArray p2shTxOuts;

    for(QVector<Payor::Channel>::const_iterator i = channels.begin(), end(channels.end()); i != end;i++)
        p2shTxOuts.append(i->json());

    QJsonObject params {
        {"fundingOutput", fundingOutPoint.toJson()},
        {"p2shTxOuts", p2shTxOuts},
        {"change", changeOutput.json()},
        {"sk", sk.toString()}
    };

    // Make call
    QJsonValue result = nodeBlockingCall("broadcast_contract", QJsonValue(params));

    // Turn string to hash
    //return TxId(result.toString());
    return;
}
