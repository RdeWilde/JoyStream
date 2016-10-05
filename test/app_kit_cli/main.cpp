/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <cli.hpp>
#include <app_kit/AppKit.hpp>
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

#include <iostream>
#include <signal.h>
#include <string>

// Explicit template instantiation of IdToString()
// used in joystream::protocol_session::exception::ConnectionAlreadyAddedException
template <>
std::string IdToString<boost::asio::ip::basic_endpoint<boost::asio::ip::tcp>>(boost::asio::ip::basic_endpoint<boost::asio::ip::tcp> const&id){
    return id.address().to_string();
}


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

uint32_t future_unix_timestamp(const unsigned int minutes) {
    std::chrono::system_clock::time_point epoch = std::chrono::system_clock::from_time_t(0);
    auto futureTime = std::chrono::system_clock::now() + std::chrono::minutes(minutes);
    auto delta = futureTime - epoch;
    return std::chrono::duration_cast<std::chrono::seconds>(delta).count();
}



int main(int argc, char *argv[])
{
    const int nsellers = 1;
    const uint64_t price = 100; //satoshis per piece
    const uint32_t locktime = future_unix_timestamp(5); // 5 minute locktime
    const uint64_t refund_fee = 20000;
    const uint64_t settlement_fee = 20000;
    const uint64_t contractFeeRate = 20000;   //satoshis/KByte - ref https://bitcoinfees.github.io/

    joystream::protocol_wire::BuyerTerms buyerTerms(price, locktime, nsellers, contractFeeRate, refund_fee);
    joystream::protocol_wire::SellerTerms sellerTerms(price, locktime, nsellers, contractFeeRate, settlement_fee);

    assert(sellerTerms.satisfiedBy(buyerTerms));
    assert(buyerTerms.satisfiedBy(sellerTerms));

    const double secondsBeforeCreatingContract = 45;
    const double secondsBeforePieceTimeout = 25;
    joystream::protocol_session::BuyingPolicy buyingPolicy(secondsBeforeCreatingContract,
                                                           secondsBeforePieceTimeout,
                                                           joystream::protocol_wire::SellerTerms::OrderingPolicy::min_price);

    if (argc != 1 && argc < 3)
    {
        std::cerr << "usage: ./app_kit_cli [buy, sell] [torrent-file]" << std::endl;
        return 1;
    }


    joystream::core::TorrentIdentifier* torrentIdentifier = nullptr;

    if(argc == 3) {
        torrentIdentifier = joystream::AppKit::makeTorrentIdentifier(argv[2]);
        if(!torrentIdentifier) {
            return 1;
        }
        std::cout << "Torrent InfoHash: " << torrentIdentifier->infoHash() << std::endl;
    }

    QCoreApplication app(argc, argv);

    std::cout << "Creating AppKit Instance\n";

    QString dataDirectory = getenv("JOYSTREAM_DATADIR") != NULL ? QString::fromStdString(getenv("JOYSTREAM_DATADIR")) : QDir::homePath();

    joystream::AppKit* kit = joystream::AppKit::createInstance(dataDirectory, Coin::Network::testnet3);

    if(!kit) {
        std::cout << "Failed to create appkit instance" << std::endl;
        return 2;
    }

    auto w = kit->wallet();

    std::vector<Coin::P2PKHAddress> addresses = w->listReceiveAddresses();

    std::string depositAddress;

    if(addresses.size() > 0){
        depositAddress = addresses.front().toBase58CheckEncoding().toStdString();
    } else {
        depositAddress = kit->wallet()->generateReceiveAddress().toBase58CheckEncoding().toStdString();
    }

    std::cout << "Wallet Deposit Address: " <<  depositAddress << std::endl;
    std::cout << "Wallet Balance: " << kit->wallet()->unconfirmedBalance() << std::endl;

    QTimer *timer = new QTimer();

    QObject::connect(timer, &QTimer::timeout, [&timer, &app, &kit](){
        kit->node()->updateStatus();

        if(!shuttingDown) return;
        timer->stop();
        std::cout << "Stopping..." << std::endl;

        kit->shutdown([&app](){
            app.quit();
        });
    });

    timer->start(1000);

    signal(SIGINT, &handleSignal);
    signal(SIGTERM, &handleSignal);

    auto startIt = [](const std::exception_ptr &eptr, joystream::core::Torrent* torrent) {
        if(eptr){
            std::rethrow_exception(eptr);
        }else {
            torrent->torrentPlugin()->start([](const std::exception_ptr &eptr){
                if(eptr){
                   std::rethrow_exception(eptr);
                }else {
                    std::cout << "Torrent Started" << std::endl;
                }
            });
        }
    };

    auto buyIt = [&kit, &buyerTerms, &buyingPolicy, &startIt](joystream::core::Torrent* torrent) {
        kit->buyTorrent(torrent, buyingPolicy, buyerTerms, [torrent, &startIt](const std::exception_ptr &eptr){
            startIt(eptr, torrent);
        });
    };

    auto sellIt = [&kit, &sellerTerms, &startIt](joystream::core::Torrent* torrent) {
        kit->sellTorrent(torrent, joystream::protocol_session::SellingPolicy(), sellerTerms, [torrent, &startIt](const std::exception_ptr &eptr){
            startIt(eptr, torrent);
        });
    };

    auto onPluginAdded = [&buyIt, &sellIt, &argv](joystream::core::Torrent* torrent) {
        // wait for torrent pluging to be added before we can go to buy/sell mode...
        QObject::connect(torrent, &joystream::core::Torrent::torrentPluginAdded, [&buyIt, &sellIt, &argv, torrent](const joystream::core::TorrentPlugin *plugin){
            // assuming this signal will only ever be emitted once
            if(std::string(argv[1]) == "buy") {
                buyIt(torrent);
            } else {
                sellIt(torrent);
            }
        });
    };

    if(torrentIdentifier) {
        std::cout << "Adding Torrent" << std::endl;

        auto savePath = (dataDirectory + QDir::separator() + "downloads").toStdString();

        kit->node()->addTorrent(0, 0, "test", std::vector<char>(), savePath, true, *torrentIdentifier,
                               [&kit, &onPluginAdded](libtorrent::error_code &ecode, libtorrent::torrent_handle &th){
            if(ecode) {
                std::cerr << "Node::addTorrent() failed: " << ecode.message().c_str() << std::endl;
                return;
            }

            // make sure to have metadata before gonig to buy or sell torrent
            auto torrent = kit->node()->torrents()[th.info_hash()];

            onPluginAdded(torrent);
        });
    }

    std::cout << "Starting Qt Application Event loop\n";
    int ret = app.exec();

    std::cout << "Exited Qt Application event loop with code: " << ret << std::endl;
    return ret;
}

