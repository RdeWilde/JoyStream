#include "BitSwaprjs.hpp"
#include "KeyPair.hpp"

#include <QProcess>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QDebug>

BitSwaprjs::BitSwaprjs()
    : _node("C:\\Program Files\\nodejs\\node.exe")
    , _module("C:\\Users\\Sindre\\Documents\\GitHub\\BitSwaprjs\\index.js") {
}

BitSwaprjs::BitSwaprjs(const QString & node, const QString & module)
    : _node(node)
    , _module(module) {
}

void BitSwaprjs::test() const {
    nodeBlockingCall("test", QJsonValue());
}

QList<KeyPair> BitSwaprjs::generate_fresh_key_pairs(int numberOfPairs) {

    // Make call to generate keys
    QJsonValue result = nodeBlockingCall("generate_fresh_key_pairs", QJsonValue(numberOfPairs));

    // Create list for key pairs
    QList<KeyPair> keyPairs;

    QJsonArray jsonKeyPairsArray = result.toArray();

    for(QJsonArray::iterator i = jsonKeyPairsArray.begin(),
        end(jsonKeyPairsArray.end()); i != end;i++) {

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
}

static Hash BitSwaprjs::compute_contract_hash(const Contract & contract) {

    // Turn into raw transaction
    QJsonObject rawTx = contract.rawTransaction();

    // Make call to generate keys
    QJsonValue result = nodeBlockingCall("compute_contract_hash", QJsonValue(rawTx));

    // Turn string to hash
    return Hash(result.toString());
}

QJsonObject BitSwaprjs::nodeBlockingCall(const QString & method, const QJsonValue & params) {

    // Build input dictionary for node script
    QJsonObject input;
    input["method"] = method;
    input["params"] = params;

    // Turn into string form
    QJsonDocument inputDocument(input);
    QByteArray byteArrayInput(inputDocument.toJson());
    QString encodedInputs = QString(byteArrayInput).replace("\"","#");

    // qDebug() << encodedInputs;

    // Build argument list
    QStringList args;
    args << "C:\\Users\\Sindre\\Documents\\GitHub\\BitSwaprjs\\index.js" << encodedInputs;

    // Allocate process and loop
    QProcess process;
    QEventLoop loop;

    // Stop event loop when processing is done
    QObject::connect(&process,
                     SIGNAL(finished(int, QProcess::ExitStatus)),
                     &loop,
                     SLOT(quit()));

    // Start process async
    process.start("C:\\Program Files\\nodejs\\node.exe", args);

    // Wait for process to be finished
    loop.exec();

    // Read what process dumped
    QByteArray rawOutput = process.readAll();

    //qDebug() << rawOutput;

    // Decode into json
    QJsonDocument outputDocument = QJsonDocument::fromJson(rawOutput);

    // Return json object
    QJsonObject o = outputDocument.object();

    if(o['state'] != 1) {
        throw std::exception("Error state returned.");
    } else {
        return o['result'];
    }
}
