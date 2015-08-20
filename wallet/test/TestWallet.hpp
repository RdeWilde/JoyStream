#ifndef TESTWALLET_HPP
#define TESTWALLET_HPP

#include <QtTest/QtTest>

#define WALLET_FILE_NAME "test-wallet"

namespace Wallet {
    class Manager;
}

class TestWallet : public QObject
{
    Q_OBJECT

    Wallet::Manager * _manager;

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

    // called after all non inittestcase
    void cleanupTestCase();
};

#endif // TESTWALLET_HPP
