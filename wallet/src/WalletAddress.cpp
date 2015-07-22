#include <wallet/WalletAddress.hpp>

WalletAddress::WalletAddress(const WalletKey & walletKey, const Coin::Address & address)
    : _walletKey(walletKey)
    , _address(address) {
}

QSqlQuery WalletAddress::createTableQuery() {

    return QSqlQuery("\
    CREATE TABLE WalletAddress (\
        walletKeyIndex  BLOB        PRIMARY KEY,\
        purpose         INTEGER     NOT NULL FOREIGN KEY REFERENCES ReceiveAddressPurpose(id),\
        requested       INTEGER     NOT NULL,\
        description     TEXT\
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

Coin::Address WalletAddress::address() const {
    return _address;
}

void WalletAddress::setAddress(const Coin::Address & address) {
    _address = address;
}