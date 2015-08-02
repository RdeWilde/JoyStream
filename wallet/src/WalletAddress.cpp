#include <wallet/WalletAddress.hpp>

#include <QSqlQuery>

WalletAddress::WalletAddress(quint64 walletKeyIndex, const Coin::P2PKHAddress & address)
    : _walletKeyIndex(walletKeyIndex)
    , _address(address) {
}

QSqlQuery WalletAddress::createTableQuery() {

    return QSqlQuery("\
    CREATE TABLE WalletAddress (\
        walletKeyIndex  INTEGER     FOREIGN KEY REFERENCES walletKeyIndex(id),\
        address         BLOB        NOT NULL UNIQUE,\
        PRIMARY KEY(walletKeyIndex)\
    )");
}

QSqlQuery WalletAddress::unboundedInsertQuery() {

    return QSqlQuery("\
                     INSERT INTO WalletAddress \
                     (walletKeyIndex, address)\
                     VALUES\
                     (:walletKeyIndex, :address)\
                     ");
}

QSqlQuery WalletAddress::insertQuery() {

    // Get templated query
    QSqlQuery query = unboundedInsertQuery();

    // bind wallet key values
    query.bindValue(":walletKeyIndex", static_cast<uint>(_walletKeyIndex));
    query.bindValue(":address", Coin::uchar_vector_to_QByteArray(_address.raw()));

    return query;
}

quint64 WalletAddress::walletKeyIndex() const {
    return _walletKeyIndex;
}

void WalletAddress::setWalletKeyIndex(quint64 walletKeyIndex) {
    _walletKeyIndex = walletKeyIndex;
}

Coin::P2PKHAddress WalletAddress::address() const {
    return _address;
}

void WalletAddress::setAddress(const Coin::P2PKHAddress & address) {
    _address = address;
}
