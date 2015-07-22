#ifndef WALLET_ADDRESS
#define WALLET_ADDRESS

#include <wallet/WalletKey.hpp>
#include <wallet/CoinWrappers.hpp>

class WalletAddress {

public:

    // Constructor from members
    WalletAddress(const WalletKey & walletKey, const Coin::Address address);

    // Constructor from dbase record
    // WalletAddress(const QSqlRecord & record);

    // Query which creates table corresponding to entity
    static QSqlQuery createTableQuery();

    // (Unbound) Query which inserts wallet key record into correspodning table
    static QSqlQuery unboundedInsertQuery();

    // Query inserting this wallet key into corresponding table
    QSqlQuery insertQuery();

    // Getters and setters
    WalletKey walletKey() const;
    void setWalletKey(const WalletKey & walletKey);

    Coin::Address address() const;
    void setAddress(const Coin::Address & address);

private:

    // Key to which address corresponds
    WalletKey _walletKey;

    // Address
    Coin::Address _address;
};

#endif // WALLET_ADDRESS
