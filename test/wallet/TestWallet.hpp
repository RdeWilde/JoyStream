#ifndef TESTWALLET_HPP
#define TESTWALLET_HPP

#include <QtTest/QtTest>

namespace Coin {
    class Transaction;
}

#define WALLET_FILE_NAME "test-wallet"
#define WALLET_SEED Coin::Seed::testSeeds[0]
#define NETWORK_TYPE Coin::Network::testnet3


namespace Wallet {
    class Manager;
}

class TestWallet : public QObject
{
    Q_OBJECT

    Wallet::Manager * _manager;

private slots:

    // called before *each* non-built in test case
    void init();

    // called after *each* non-built in test case
    void cleanup();

    /**
     * Test routines
     */

    //
    void createWallet();

    //void outPoint_data();
    void outPoint();

    //void input_data();
    void input();

    void key();

    void address();

    void tx();

    void listutxo();

    void lockutxo();

public:

    Coin::Transaction createWalletTx(quint32 numberOfOutputs);
};

#endif // TESTWALLET_HPP
