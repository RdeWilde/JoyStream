#include "BitSwaprjs.hpp"
#include "KeyPair.hpp"
#include "P2PKHTxOut.hpp"
#include "P2SHTxOut.hpp"
#include "UnspentP2PKHOutput.hpp"

//#include "extension/PaymentChannel/Contract.hpp"
//#include "extension/PaymentChannel/Refund.hpp"


#include "Signature.hpp"

#include <QProcess>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QDebug>

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

    if(!successValue.toBool()) {
        qDebug() << o["result"].toString();
        throw std::exception("Error state returned.");
    } else
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

TxId BitSwaprjs::compute_contract_hash(const UnspentP2PKHOutput & utxo, const QVector<P2SHTxOut> & contractOutputs, const P2PKHTxOut & changeOutput) {

//compute_contract_hash(const OutPoint & fundingOutPoint, const quint64 fundingValue, const PrivateKey & sk, const QVector<P2SHTxOut> & contractOutputs, const P2PKHTxOut & changeOutput) {

    // Encode parameters into json
    QJsonArray p2shTxOuts;

    for(QVector<P2SHTxOut>::const_iterator i = contractOutputs.constBegin(),
        end(contractOutputs.constEnd()); i != end;i++)
        p2shTxOuts.append(i->json());

    QJsonObject params {
        //{"fundingOutput", fundingOutPoint.json()},
        //{"fundingValue", static_cast<int>(fundingValue)},
        //{"sk", sk.toString()}
        {"utxo", utxo.json()},
        {"p2shTxOuts", p2shTxOuts},
        {"change", changeOutput.json()},
    };

    // Make call
    QJsonValue result = nodeBlockingCall("compute_contract_hash", QJsonValue(params));

    // Turn string to hash
    return TxId(result.toString());
}

Signature BitSwaprjs::compute_refund_signature(const OutPoint & contractOutputPoint, const PrivateKey &sk, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, quint32 refundLockTime) {

    return Signature();

    // Create parameters
    QJsonObject params {
        {"contractOutputPoint", contractOutputPoint.json()},
        {"sk", sk.toString()},
        {"firstPk", firstPk.toString()},
        {"secondPk", secondPk.toString()},
        {"refundOutput", refundOutput.json()},
        {"refundLockTime", static_cast<qint64>(refundLockTime)}
    };

    // Make call
    QJsonValue result = nodeBlockingCall("compute_contract_output_spending_signature", QJsonValue(params));

    // Turn string to signature
    return Signature(result.toString());
}

Signature BitSwaprjs::compute_payment_signature(const OutPoint & contractOutputPoint, const PrivateKey &sk, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, const P2PKHTxOut & paymentOutput) {

    return Signature();

    // Create parameters
    QJsonObject params {
        {"contractOutputPoint", contractOutputPoint.json()},
        {"sk", sk.toString()},
        {"firstPk", firstPk.toString()},
        {"secondPk", secondPk.toString()},
        {"refundOutput", refundOutput.json()},
        {"paymentOutput", paymentOutput.json()}
    };

    // Make call
    QJsonValue result = nodeBlockingCall("compute_contract_output_spending_signature", QJsonValue(params));

    // Turn string to signature
    return Signature(result.toString());
}

bool BitSwaprjs::check_refund_signatures(const OutPoint & contractOutputPoint, const Signature & payorSignature, const Signature & payeeSignature, const PublicKey & firstPk, const PublicKey & secondPk, const P2PKHTxOut & refundOutput, quint32 refundLockTime) {

    return true;

    // Create parameters
    QJsonObject params {
        {"contractOutputPoint", contractOutputPoint.json()},
        {"payorSignature", payorSignature.toString()},
        {"payeeSignature", payeeSignature.toString()},
        {"firstPk", firstPk.toString()},
        {"secondPk", secondPk.toString()},
        {"refundOutput", refundOutput.json()},
        {"refundLockTime", static_cast<qint64>(refundLockTime)}
    };

    // Make call
    QJsonValue result = nodeBlockingCall("check_contract_output_spending_signatures", QJsonValue(params));

    // Turn into bool result
    return result.toBool();
}

bool BitSwaprjs::check_payment_signatures(const OutPoint & contractOutputPoint, const Signature &payorSignature, const Signature &payeeSignature, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, const P2PKHTxOut & paymentOutput) {

    return true;

    // Create parameters
    QJsonObject params {
        {"contractOutputPoint", contractOutputPoint.json()},
        {"payorSignature", payorSignature.toString()},
        {"payeeSignature", payeeSignature.toString()},
        {"firstPk", firstPk.toString()},
        {"secondPk", secondPk.toString()},
        {"refundOutput", refundOutput.json()},
        {"paymentOutput", paymentOutput.json()}
    };

    // Make call
    QJsonValue result = nodeBlockingCall("check_contract_output_spending_signatures", QJsonValue(params));

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

TxId BitSwaprjs::broadcast_contract(const UnspentP2PKHOutput & utxo, const QVector<P2SHTxOut> & contractOutputs, const P2PKHTxOut & changeOutput) {
//(const OutPoint & fundingOutPoint, const PrivateKey & sk, const QVector<P2SHTxOut> & contractOutputs, const P2PKHTxOut & changeOutput) {

    // Encode parameters into json
    QJsonArray p2shTxOuts;

    for(QVector<P2SHTxOut>::const_iterator i = contractOutputs.constBegin(),
        end(contractOutputs.constEnd()); i != end;i++)
        p2shTxOuts.append(i->json());

    QJsonObject params {
        {"utxo", utxo.json()},
        {"p2shTxOuts", p2shTxOuts},
        {"change", changeOutput.json()},
    };

    // Make call
    QJsonValue result = nodeBlockingCall("broadcast_contract", QJsonValue(params));

    Q_ASSERT(result.isObject());

    QJsonObject resultObject = result.toObject();

    Q_ASSERT(resultObject.contains("transaction_hash"));

    QString transaction_hash = resultObject["transaction_hash"].toString();

    // Turn string to bool
    return TxId(transaction_hash);
}


bool BitSwaprjs::broadcast_refund(const OutPoint & contractOutputPoint, const Signature &payorSignature, const Signature &payeeSignature, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, quint32 refundLockTime) {

    return true;

    // Create parameters
    QJsonObject params {
        {"contractOutputPoint", contractOutputPoint.json()},
        {"payorSignature", payorSignature.toString()},
        {"payeeSignature", payeeSignature.toString()},
        {"firstPk", firstPk.toString()},
        {"secondPk", secondPk.toString()},
        {"refundOutput", refundOutput.json()},
        {"refundLockTime", static_cast<qint64>(refundLockTime)}
    };

    // Make call
    QJsonValue result = nodeBlockingCall("broadcast_contract_output_spending_tx", QJsonValue(params));

    // Turn into bool result
    return result.toBool();
}

bool BitSwaprjs::broadcast_payment(const OutPoint & contractOutputPoint, const Signature &payorSignature, const Signature &payeeSignature, const PublicKey &firstPk, const PublicKey &secondPk, const P2PKHTxOut &refundOutput, const P2PKHTxOut & paymentOutput) {

    return true;

    // Create parameters
    QJsonObject params {
        {"contractOutputPoint", contractOutputPoint.json()},
        {"payorSignature", payorSignature.toString()},
        {"payeeSignature", payeeSignature.toString()},
        {"firstPk", firstPk.toString()},
        {"secondPk", secondPk.toString()},
        {"refundOutput", refundOutput.json()},
        {"paymentOutput", paymentOutput.json()}
    };

    // Make call
    QJsonValue result = nodeBlockingCall("broadcast_contract_output_spending_tx", QJsonValue(params));

    // Turn into bool result
    return result.toBool();
}

quint64 BitSwaprjs::get_tx_outpoint(const OutPoint & point, bool & spent) {

    // Encode parameters into json
    QJsonObject params {
        {"OutPoint", point.json()},
    };

    // Make call
    QJsonValue result = nodeBlockingCall("get_tx_outpoint", QJsonValue(params));

    // Parse results
    QJsonObject nodeResult = result.toObject();

    quint64 value = nodeResult["value"].toDouble();
    spent = nodeResult["spent"].toBool();

    return value;
}
