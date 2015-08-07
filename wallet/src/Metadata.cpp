
#include <wallet/Metadata.hpp>

#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

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

void Metadata::populateTable(QSqlDatabase db, const QByteArray & seed, Coin::Network network, const QDateTime & created) {

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

    uint createdUint = static_cast<uint>(created.toMSecsSinceEpoch());
    insertQuery.bindValue(":value", QByteArray::number(createdUint));
    Q_ASSERT(insertQuery.exec());
        //throw std::runtime_error("Could not insert network key-value.");
}

Coin::Network Metadata::getNetwork(QSqlDatabase db) {

}

void Metadata::setNetwork(QSqlDatabase db, Coin::Network network) {

}

QByteArray Metadata::getSeed(QSqlDatabase db) {

}

void Metadata::setSeed(QSqlDatabase db, const QByteArray & seed) {

}

QDateTime Metadata::getCreated(QSqlDatabase db) {

}

void Metadata::setCreated(QSqlDatabase db, const QDateTime & created) {

}

QByteArray Metadata::encodeNetwork(Coin::Network network) {

    switch(network) {
        case Coin::Network::testnet3: return QByteArray::number(0);
        case Coin::Network::mainnet: return QByteArray::number(1);
        default:
                Q_ASSERT(false);
    }
}

Coin::Network Metadata::decodeNetwork(const QByteArray & blob) {

    bool ok;
    uint encodedNetwork = blob.toUInt(&ok);

    Q_ASSERT(ok);

    switch(encodedNetwork) {

        case 0: return Coin::Network::testnet3;
        case 1: return Coin::Network::mainnet;
        default:
            Q_ASSERT(false);
    }
}
