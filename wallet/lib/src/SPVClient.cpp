/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <wallet/SPVClient.hpp>
#include <common/Network.hpp>
#include <common/P2PKHAddress.hpp>

#include <CoinQ/CoinQ_coinparams.h>

#include <QString>
#include <QSet>


namespace Wallet {

SPVClient::SPVClient(const QString & blockHeaderStore,
                       Coin::Network network,
                       const QSet<Coin::P2PKHAddress> & addresses,
                       const QSet<Coin::typesafeOutPoint> & outPoints)
    : _filter(addresses.size() + outPoints.size(), 0.001, 0, 0) // <-- factor out into settings class
    , _spvP2PClient(CoinQ::NetworkSelector(network == Coin::Network::testnet3 ? "testnet3" : "bitcoin").getCoinParams()) {

    // Load headers from block store
    _spvP2PClient.loadHeaders(blockHeaderStore.toStdString(), false, [&](const CoinQBlockTreeMem& blocktree) {

        std::cout << "Best height: " << blocktree.getBestHeight() << " Total work: " << blocktree.getTotalWork().getDec() << std::endl;

        // Send signal: must be emitted on spesific object
        emit blockStoreLoaded(blocktree);

        return true; // !g_bShutdown; <--- what is this for?
    });

    //
    // Subscribe to spv cliente vents
    //
/**
    _spvP2PClient.subscribeStarted([&]() {
       std::cout << std::endl << " started." << std::endl;
    });

    _spvP2PClient.subscribeStopped([&]() {
       std::cout << std::endl << " stopped." << std::endl;
    });

    _spvP2PClient.subscribeOpen([&]() {
       std::cout << std::endl << " open." << std::endl;
    });

    _spvP2PClient.subscribeClose([&]() {
       std::cout << std::endl << " closed." << std::endl;
    });

    _spvP2PClient.subscribeTimeout([&]() {
       std::cout << std::endl << " timeout." << std::endl;
    });

    _spvP2PClient.subscribeConnectionError([&](const string& error, int code) {
       std::cout << std::endl << " connection error: " << error << std::endl;
    });

    _spvP2PClient.subscribeProtocolError([&](const string& error, int code) {
       std::cout << std::endl << " protocol error: " << error << std::endl;
    });

    _spvP2PClient.subscribeBlockTreeError([&](const string& error, int code) {
       std::cout << std::endl << " block tree error: " << error << std::endl;
    });

    _spvP2PClient.subscribeSynchingHeaders([&]() {
       std::cout << std::endl << " fetching headers." << std::endl;
    });

    _spvP2PClient.subscribeHeadersSynched([&]() {
       std::cout << std::endl << " headers synched." << std::endl;
       hashvector_t hashes;
       _spvP2PClient.syncBlocks(hashes, time(NULL) - 10*60*60); // Start 10 hours earlier
    });

    _spvP2PClient.subscribeSynchingBlocks([&]() {
       std::cout << std::endl << " fetching blocks." << std::endl;
    });

    _spvP2PClient.subscribeBlocksSynched([&]() {
       std::cout << std::endl << " blocks synched." << std::endl;

       std::cout << std::endl << "Fetching mempool..." << std::endl;
       _spvP2PClient.getMempool();
    });

    _spvP2PClient.subscribeStatus([&](const string& status) {
       std::cout << std::endl << " status: " << status << std::endl;
    });

    _spvP2PClient.subscribeNewTx([&](const Transaction& tx) {
       std::cout << std::endl << "NEW TX: " << tx.getHashLittleEndian().getHex() << std::endl;
    });

    _spvP2PClient.subscribeMerkleTx([&](const ChainMerkleBlock& merkleBlock, const Transaction& tx, unsigned int txIndex, unsigned int txTotal)
    {
       std::cout << "  tx (" << txIndex << "/" << (txTotal - 1) << "): " << tx.getHashLittleEndian().getHex() << std::endl;
    });

    _spvP2PClient.subscribeBlock([&](const ChainBlock& block) {
       std::cout << std::endl << "NEW BLOCK: " << block.blockHeader.getHashLittleEndian().getHex() << " height: " << block.height << std::endl;
    });

    _spvP2PClient.subscribeMerkleBlock([&](const ChainMerkleBlock& merkleblock) {

        std::cout << std::endl << "NEW MERKLE BLOCK" << std::endl
                    << "  hash: " << merkleblock.blockHeader.getHashLittleEndian().getHex() << std::endl
                    << "  height: " << merkleblock.height << std::endl;

        try
        {
            PartialMerkleTree tree(merkleblock.nTxs, merkleblock.hashes, merkleblock.flags, merkleblock.merkleRoot());
            std::vector<uchar_vector> txhashes = tree.getTxHashesLittleEndianVector();
            unsigned int i = 0;
            std::cout << "should contain txs:" << std::endl;
            for (auto& txhash: txhashes) { std::cout << "  tx " << i++ << ": " << uchar_vector(txhash).getHex() << std::endl; }
        }
        catch (const exception& e)
        {
           std::cout << "Error constructing partial merkle tree: " << e.what() << std::endl;
        }

        std::cout << "--------------------" << std::endl;
        blockTxIndex = 0;
    });

    _spvP2PClient.subscribeAddBestChain([&](const ChainHeader& header) {
       std::cout << std::endl << " added to best chain: " << header.getHashLittleEndian().getHex() << " height: " << header.height << std::endl;
    });

    _spvP2PClient.subscribeRemoveBestChain([&](const ChainHeader& header) {
       std::cout << std::endl << " removed from best chain: " << header.getHashLittleEndian().getHex() << " height: " << header.height << std::endl;
    });

    _spvP2PClient.subscribeBlockTreeChanged([&]() {
       std::cout << std::endl << " block tree changed." << std::endl;
    });
    */

    // Add addresses to the bloom filter
    for(QSet<Coin::P2PKHAddress>::const_iterator i = addresses.constBegin(),
        end = addresses.constEnd();
        i != end;
        i++);
        //addAddress(*i);

    // Add addresses to the bloom filter
    for(QSet<Coin::typesafeOutPoint>::const_iterator i = outPoints.constBegin(),
        end = outPoints.constEnd();
        i != end;
        i++);
        //addOutPoint(*i);

    // Set filter to be used by p2p client
    _spvP2PClient.setBloomFilter(_filter);
}

void SPVClient::start(const QString & host) {

    // Start client
    _spvP2PClient.start(host.toStdString(), _spvP2PClient.getCoinParams().default_port());

    /**
    std::cout << endl << "Connecting to " << coinParams.network_name() << " peer" << endl
         << "-------------------------------------------" << endl
         << "  host:             " << host << endl
         << "  port:             " << port << endl
         << "  magic bytes:      " << hex << coinParams.magic_bytes() << endl
         << "  protocol version: " << dec << coinParams.protocol_version() << endl
         << endl;
    */
}

void SPVClient::stop() {
    _spvP2PClient.stop();
}

void SPVClient::addAddress(const Coin::P2PKHAddress & address) {

}

void SPVClient::addOutPoint(const Coin::typesafeOutPoint & outPoint) {

}

void SPVClient::broadcast(const Coin::Transaction & tx) {

}

}
