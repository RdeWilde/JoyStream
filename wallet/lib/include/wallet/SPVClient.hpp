/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef WALLET_SPVManager_HPP
#define WALLET_SPVManager_HPP

#include <CoinQ/CoinQ_netsync.h>
#include <CoinCore/BloomFilter.h>

#include <QObject>

class QString;

namespace Coin {

    enum class Network;
    class P2PKHAddress;
    class typesafeOutPoint;
}

namespace Wallet {

class SPVClient : public QObject
{
    Q_OBJECT

public:

    SPVClient(const QString & blockHeaderStore,
               Coin::Network network,
               const QSet<Coin::P2PKHAddress> & addresses,
               const QSet<Coin::typesafeOutPoint> & outPoints);

signals:

    // Block store was loaded from file
    void blockStoreLoaded(const CoinQBlockTreeMem & blocktree);

    // Has received the given transaction from a peer
    void transactionArrived(const Coin::Transaction & tx);

public slots:

    // Start client
    void start(const QString & host);

    // Stops the client
    void stop();

    // Adds given address to the bloom filter
    void addAddress(const Coin::P2PKHAddress & address);

    // Adds given outpoint to the bloom filter
    void addOutPoint(const Coin::typesafeOutPoint & outPoint);

    // Broadcast given transaction
    void broadcast(const Coin::Transaction & tx);



private:

    // Bloom filter used
    Coin::BloomFilter _filter;

    // Talks with main chain
    CoinQ::Network::NetworkSync _spvP2PClient;
};

}

#endif // WALLET_SPVManager_HPP
