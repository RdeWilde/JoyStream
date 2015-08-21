#ifndef TESTWALLET_HPP
#define TESTWALLET_HPP

#include <QtTest/QtTest>
//#include <CoinCore/hdkeys.h>

#include <QSqlDatabase>

#define WALLET_FILE_NAME "test-wallet"
#define NETWORK_TYPE Coin::Network::testnet3

namespace Wallet {
    class Manager;
}

class TestWallet : public QObject
{
    Q_OBJECT

    Wallet::Manager * _manager;

    // Database used by wallet
    QSqlDatabase _db;
    //Coin::HDKeychain _keyChain;

private slots:

    // called before everything else
    void initTestCase();

    /**
     * Routines for adding and searching for the given data type to wallet
     */

    //void outPoint_data();
    void outPoint();

    //void input_data();
    void input();

    void key();

    void address();

    void tx();

    void listutxo();


    // called after all non inittestcase
    void cleanupTestCase();
};

#endif // TESTWALLET_HPP
