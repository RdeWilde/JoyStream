/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <cli.hpp>
#include <app_kit/kit.hpp>

#include <bitcoin/SPVWallet.hpp>
#include <common/P2PKHAddress.hpp>
#include <core/core.hpp>

#include <protocol_session/Exceptions.hpp>
#include <common/Network.hpp>

#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/asio/impl/src.hpp>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <ctime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>

#include <iostream>
#include <signal.h>
#include <string>

#include "buyqueue.hpp"
#include "sellqueue.hpp"

bool shuttingDown = false;
int shutdownAttempts = 0;

void handleSignal(int sig)
{
    shuttingDown = true;
    std::cout << "Got Quit Signal" << std::endl;
    if(++shutdownAttempts > 2) {
        std::cerr << "Application Not Exiting - Force Quit" << std::endl;
        exit(1);
    }
}

std::string stateToString(libtorrent::torrent_status::state_t state) {
    switch (state) {
        case libtorrent::torrent_status::state_t::allocating: return "allocating";
        case libtorrent::torrent_status::state_t::checking_files: return "checking_files";
        case libtorrent::torrent_status::state_t::checking_resume_data: return "checking_resume_data";
        case libtorrent::torrent_status::state_t::downloading: return "downloading";
        case libtorrent::torrent_status::state_t::downloading_metadata: return "downloading_metadata";
        case libtorrent::torrent_status::state_t::finished: return "finished";
        case libtorrent::torrent_status::state_t::seeding: return "seeding";
    }

    throw std::runtime_error("invalid state");
}

int64_t sumOfOutputs(const std::vector<joystream::bitcoin::Store::StoreControlledOutput> &standardOutputs) {
    return std::accumulate(standardOutputs.begin(), standardOutputs.end(), (int64_t)0,
                           [](int64_t &sum, joystream::bitcoin::Store::StoreControlledOutput output) -> int64_t {
        return sum + output.value;
    });
}

void dumpWalletInfo(joystream::bitcoin::SPVWallet *wallet) {

    std::vector<Coin::P2PKHAddress> addresses = wallet->listReceiveAddresses();

    std::string depositAddress;

    if(addresses.size() > 0){
        depositAddress = addresses.front().toBase58CheckEncoding().toStdString();
    } else {
        depositAddress = wallet->generateReceiveAddress().toBase58CheckEncoding().toStdString();
    }

    std::cout << "Wallet Deposit Address: " <<  depositAddress << std::endl;

    std::cout << "Wallet Balance: " << wallet->unconfirmedBalance() << std::endl;

    auto outputChannels = joystream::appkit::util::outputsToOutboundPaymentChannelCommitments(wallet->getNonStandardStoreControlledOutputs());
    auto inputChannels = joystream::appkit::util::outputsToInboundPaymentChannelCommitments(wallet->getNonStandardStoreControlledOutputs());

    int64_t outgoingLockedFunds = std::accumulate(outputChannels.begin(), outputChannels.end(), (int64_t)0,
                                                  [](int64_t &sum, joystream::paymentchannel::Commitment &commitment) -> int64_t {
        return sum + commitment.value();
    });

    int64_t incomingLockedFunds = std::accumulate(inputChannels.begin(), inputChannels.end(), (int64_t)0,
                                                  [](int64_t &sum, joystream::paymentchannel::Commitment &commitment) -> int64_t {
        return sum + commitment.value();
    });

    std::cout << "Wallet funds locked in outbound payment channels: " << outgoingLockedFunds << std::endl;
    std::cout << "Wallet potential unsettled funds in inbound payment channels: " << incomingLockedFunds << std::endl;
    std::cout << "Wallet spendable P2PKH balance: " << sumOfOutputs(wallet->getStandardStoreControlledOutputs()) << std::endl;

}

int main(int argc, char *argv[])
{
    const int nsellers = 1;
    const uint64_t price = 100; //satoshis per piece

    const uint32_t locktime = 5;
    const uint64_t settlement_fee = 5000;
    const uint64_t contractFeeRate = 20000;   //satoshis/KByte - ref https://bitcoinfees.github.io/

    joystream::protocol_wire::BuyerTerms buyerTerms(price, locktime, nsellers, contractFeeRate);
    joystream::protocol_wire::SellerTerms sellerTerms(price, locktime, nsellers, contractFeeRate, settlement_fee);

    assert(sellerTerms.satisfiedBy(buyerTerms));
    assert(buyerTerms.satisfiedBy(sellerTerms));

    const double secondsBeforeCreatingContract = 3;
    const double secondsBeforePieceTimeout = 25;
    joystream::protocol_session::BuyingPolicy buyingPolicy(secondsBeforeCreatingContract,
                                                           secondsBeforePieceTimeout,
                                                           joystream::protocol_wire::SellerTerms::OrderingPolicy::min_price);

    joystream::core::TorrentIdentifier* torrentIdentifier = nullptr;
    joystream::appkit::Settings settings;


    std::string command;

    if(argc > 1) command = argv[1];

    if(argc > 2 && (command == "buy" || command == "sell")) {
        torrentIdentifier = joystream::appkit::util::makeTorrentIdentifier(argv[2]);
        if(!torrentIdentifier) {
            return 2;
        }
        std::cout << "Torrent InfoHash: " << torrentIdentifier->infoHash() << std::endl;
    } else if(argc > 1 && command == "info"){
        settings.autoStartWalletSync = false;
    }

    if(getenv("DEBUG") != NULL) {
        INIT_LOGGER("netsync.log");
    }

    QCoreApplication app(argc, argv);

    std::cout << "Creating AppKit Instance\n";

    std::string kitDataDirectory = getenv("JOYSTREAM_DATADIR") != NULL ? getenv("JOYSTREAM_DATADIR") : QDir::homePath().toStdString();
    joystream::appkit::DataDirectory dir(kitDataDirectory);

    dir.lock();

    joystream::appkit::AppKit* kit = joystream::appkit::AppKit::create(dir.walletFilePath().toStdString(),
                                                                       dir.blockTreeFilePath().toStdString(),
                                                                       Coin::Network::testnet3,
                                                                       settings);

    if(!kit) {
        std::cout << "Failed to create appkit instance" << std::endl;
        return 3;
    }

    dumpWalletInfo(kit->wallet());

    if(command == "info") {
        dir.unlock();
        return 0;
    }

    // Attempt to claim refunds
    for(auto refund : joystream::appkit::util::outputsToRefunds(kit->wallet()->getNonStandardStoreControlledOutputs())) {
        // suggested improvements - we should check that the locktime has expired
        //                        - combine multiple refunds into one transaction
        auto destination = kit->wallet()->generateReceiveAddress();
        auto tx = refund.getSignedSpendingTransaction(destination, 5000);
        kit->broadcastTransaction(tx);
        std::cout << "Broadcasting Refund Tx: " << Coin::TransactionId::fromTx(tx).toRPCByteOrder() << std::endl;
    }

    auto saveTorrents = [&dir, kit]() {
        joystream::appkit::SavedTorrents savedTorrents = kit->generateSavedTorrents();
        QJsonObject rootObj;
        rootObj["torrents"] = savedTorrents.toJson();
        QJsonDocument doc(rootObj);
        auto savePath = dir.savedTorrentsFilePath();
        QFile saveFile(savePath);
        saveFile.open(QIODevice::WriteOnly);
        auto data = doc.toJson();
        saveFile.write(data);
        saveFile.close();
    };

    auto loadTorrents = [&dir, kit]() -> joystream::appkit::SavedTorrents {
        auto savePath = dir.savedTorrentsFilePath();
        QFile saveFile(savePath);
        saveFile.open(QIODevice::ReadOnly);
        auto data = saveFile.readAll();
        saveFile.close();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        auto savedTorrents = doc.object()["torrents"];

        if(!savedTorrents.isNull())
            return joystream::appkit::SavedTorrents(savedTorrents);

        return joystream::appkit::SavedTorrents();
    };

    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [&timer, &app, &kit, &saveTorrents](){
        if(!shuttingDown) {
            return;
        }

        timer.stop();

        saveTorrents();

        std::cout << "Stopping..." << std::endl;
        kit->shutdown([&app](){
            app.quit();
        });
    });

    timer.start(500);

    signal(SIGINT, &handleSignal);
    signal(SIGTERM, &handleSignal);

    BuyQueue buyQueue(kit);
    SellQueue sellQueue(kit);

    // Load Saved Torrents
    joystream::appkit::SavedTorrents savedTorrents = loadTorrents();

    for(const auto &torrent : savedTorrents.torrents()) {
        libtorrent::sha1_hash infoHash = torrent.first;
        joystream::appkit::SavedTorrentParameters torrentParams = torrent.second;
        joystream::appkit::SavedSessionParameters sessionParams = torrentParams.sessionParameters();

        if(sessionParams.mode() == joystream::protocol_session::SessionMode::buying) {
            buyQueue.add(infoHash, sessionParams.buyerTerms(), sessionParams.buyingPolicy(), sessionParams.state());
        } else if(sessionParams.mode() == joystream::protocol_session::SessionMode::selling) {
            sellQueue.add(infoHash, sessionParams.sellerTerms(), sessionParams.sellingPolicy(), sessionParams.state());
        }

        try {
            kit->addTorrent(torrentParams, [](libtorrent::error_code &ecode, libtorrent::torrent_handle &th){

                if(ecode) {
                    std::cerr << "addTorrent failed: " << ecode.message().c_str() << std::endl;
                }

                std::cout << "Torrent Starting Status:" << stateToString(th.status().state) << std::endl;
            });

        } catch(std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    if(torrentIdentifier) {
        std::cout << "Adding Torrent" << std::endl;

        if(command == "buy")
            buyQueue.add(torrentIdentifier->infoHash(), buyerTerms, buyingPolicy);

        if(command == "sell")
            sellQueue.add(torrentIdentifier->infoHash(), sellerTerms, joystream::protocol_session::SellingPolicy());

        try {
            kit->node()->addTorrent(0, 0,
                                    libtorrent::to_hex(torrentIdentifier->infoHash().to_string()),
                                    std::vector<char>(),
                                    dir.defaultSavePath().toStdString(), true, *torrentIdentifier,
                                    [](libtorrent::error_code &ecode, libtorrent::torrent_handle &th){

                if(ecode) {
                    std::cerr << "addTorrent failed: " << ecode.message().c_str() << std::endl;
                }

                std::cout << "Torrent Starting Status:" << stateToString(th.status().state) << std::endl;
            });



        } catch(std::exception &e) {
            std::cout << e.what() << std::endl;
        }

    }

    std::cout << "Starting Qt Application Event loop\n";
    int ret = app.exec();

    dir.unlock();

    std::cout << "Exited Qt Application event loop with code: " << ret << std::endl;
    return ret;
}

