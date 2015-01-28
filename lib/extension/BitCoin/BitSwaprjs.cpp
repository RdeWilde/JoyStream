#include "BitSwaprjs.hpp"

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

void BitSwaprjs::generate_fresh_key_pairs(int numberOfPairs) const {
    QJsonObject o = nodeBlockingCall("generate_fresh_key_pairs", QJsonValue(numberOfPairs));

    if(o['state'] != 1) {
        qDebug() << "Bad state";
    } else {

        qDebug() << "Got these key-pairs";

        o.
        for()


    }
}

QJsonObject BitSwaprjs::nodeBlockingCall(const QString & method, const QJsonValue & params) const {

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
    args << _module << encodedInputs;

    // Allocate process and loop
    QProcess process;
    QEventLoop loop;

    // Stop event loop when processing is done
    QObject::connect(&process,
                     SIGNAL(finished(int, QProcess::ExitStatus)),
                     &loop,
                     SLOT(quit()));

    // Start process async
    process.start(_node, args);

    // Wait for process to be finished
    loop.exec();

    // Read what process dumped
    QByteArray rawOutput = process.readAll();

    //qDebug() << rawOutput;

    // Decode into json
    QJsonDocument outputDocument = QJsonDocument::fromJson(rawOutput);

    // Return json object
    return outputDocument.object();
}
