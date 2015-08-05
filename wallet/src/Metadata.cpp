
#include <wallet/Metadata.hpp>

#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlQuery>

const QByteArray Metadata::_networkKey = QByteArray("network");
const QByteArray Metadata::_seedKey = QByteArray("seed");
const QByteArray Metadata::_createdKey = QByteArray("created");

void Metadata::createTable(QSqlDatabase db) {

    QSqlQuery result = db.exec("\
    CREATE TABLE Metadata (\
        key BLOB,\
        value BLOB,\
        PRIMARY KEY(key),\
    )");


}

void Metadata::populateTable(QSqlDatabase db, const QByteArray & seed, Network network, const QDateTime & created) {

    // Create query
    QSqlQuery insertQuery("INSERT INTO Metadata (key, value) VALUES (:key, :value)", db);

    // network
    insertQuery.bindValue(":key", _networkKey);
    insertQuery.bindValue(":value", encodeNetwork(network));
    Q_ASSERT(insertQuery.exec());
        //throw std::runtime_error("Could not insert network key-value.");

    // network
    insertQuery.bindValue(":key", _seedKey);
    insertQuery.bindValue(":value", seed);
    Q_ASSERT(insertQuery.exec());
        //throw std::runtime_error("Could not insert seed key-value.");

    // network
    insertQuery.bindValue(":key", _createdKey);
    insertQuery.bindValue(":value", QByteArray(created.toMSecsSinceEpoch()));
    Q_ASSERT(insertQuery.exec());
        //throw std::runtime_error("Could not insert network key-value.");
}

Network Metadata::getNetwork(QSqlDatabase db) {

}

void Metadata::setNetwork(QSqlDatabase db, Network network) {

}

QByteArray Metadata::getSeed(QSqlDatabase db) {

}

void Metadata::setSeed(QSqlDatabase db, const QByteArray & seed) {

}

QDateTime Metadata::getCreated(QSqlDatabase db) {

}

void Metadata::setCreated(QSqlDatabase db, const QDateTime & created) {

}

QByteArray Metadata::encodeNetwork(Network network) {

    switch(network) {
        case Network::testnet3: return QByteArray(0);
        case Network::mainnet: return QByteArray(1);
        default:
                Q_ASSERT(false);
    }
}

Network Metadata::decodeNetwork(const QByteArray & blob) {

    bool ok;
    uint encodedNetwork = blob.toUInt(ok);

    Q_ASSERT(ok);

    switch(encodedNetwork) {

        case 0: return Network::testnet3;
        case 1: return Network::mainnet;
        default:
            Q_ASSERT(false);
    }
}
