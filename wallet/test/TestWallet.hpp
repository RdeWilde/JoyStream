#ifndef TESTWALLET_HPP
#define TESTWALLET_HPP

#include <QtTest/QtTest>

#define WALLET_FILE_NAME "test-wallet"

class TestWallet : public QObject
{
    Q_OBJECT

private slots:

    // called before everything else
    void initTestCase();

    void inputs();

    // called after all non inittestcase
    void cleanupTestCase();
};

#endif // TESTWALLET_HPP
