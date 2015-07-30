#include <wallet/WalletAddress.hpp>

#include <QSqlQuery>

WalletAddress::WalletAddress(const WalletKey & walletKey, const Coin::P2PKHAddress & address)
    : _walletKey(walletKey)
    , _address(address) {
}

QSqlQuery WalletAddress::createTableQuery() {

    return QSqlQuery("\
    CREATE TABLE WalletAddress (\
        walletKeyIndex  BLOB        FOREIGN KEY REFERENCES walletKeyIndex(id),\
        address         BLOB        NOT NULL UNIQUE,\
        PRIMARY KEY()\
    )");
}

QSqlQuery WalletAddress::unboundedInsertQuery() {

}

QSqlQuery WalletAddress::insertQuery() {

}

WalletKey WalletAddress::walletKey() const {
    return _walletKey;
}

void WalletAddress::setWalletKey(const WalletKey & walletKey) {
    _walletKey = walletKey;
}

Coin::P2PKHAddress WalletAddress::address() const {
    return _address;
}

void WalletAddress::setAddress(const Coin::P2PKHAddress & address) {
    _address = address;
}
