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

int main(int argc, char *argv[])
{
    const int nsellers = 1;
    const uint64_t price = 10; //satoshis per piece

    //Fixed locktime is problematic when peers are trying to match satisfying terms
    //because the locktime is calculated once, and each peer will calculate the locktime at differnet times.
    //locktime should really be relative to the time when the contract is broadcasted
    //This is the main reason we want to update our payment channel to use check sequence verify rather than check locktime verify
    //even using block height has the same problem... as time passes buyer waiting for sellers will find less and less potential candidates
    //and at the point where the locktime is "in the past".. no sellers will satisfy the terms
    //const uint32_t locktime = future_unix_timestamp(5); // 5 minute locktime
    const uint32_t locktime = 0; //setting to zero just for testing for now
    const uint64_t refund_fee = 0; // refund fee is remeniscent of old protocol is it still needed?
    const uint64_t settlement_fee = 0;
    const uint64_t contractFeeRate = 20;   //satoshis/KByte - ref https://bitcoinfees.github.io/

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
        if(!shuttingDown) {
            return;
        }

        timer->stop();

        std::cout << "Stopping..." << std::endl;
        kit->shutdown([&app](){
            app.quit();
        });
    });

    timer->start(500);

    signal(SIGINT, &handleSignal);
    signal(SIGTERM, &handleSignal);

    auto startIt = [](joystream::core::Torrent* torrent) {
        torrent->torrentPlugin()->start([](const std::exception_ptr &eptr){
            if(eptr){
               std::rethrow_exception(eptr);
            }else {
                std::cout << "Torrent Started" << std::endl;
            }
        });
    };

    auto buyIt = [&kit, &buyerTerms, &buyingPolicy, &startIt](joystream::core::Torrent* torrent) {
        kit->buyTorrent(torrent, buyingPolicy, buyerTerms, [torrent, &startIt](const std::exception_ptr &eptr){
            if(eptr){
                // unable to go to buy mode, try again in 5 seconds..?
            } else {
                startIt(torrent);
            }
        });
    };

    auto sellIt = [&kit, &sellerTerms, &startIt](joystream::core::Torrent* torrent) {
        kit->sellTorrent(torrent, joystream::protocol_session::SellingPolicy(), sellerTerms, [torrent, &startIt](const std::exception_ptr &eptr){
            if(eptr){
                // unable to go to sell mode, try again in 5 seconds..?
            } else {
                startIt(torrent);
            }
        });
    };

    // wait for torrent to be added
    QObject::connect(kit->node(), &joystream::core::Node::addedTorrent, [&buyIt, &sellIt, &argv](joystream::core::Torrent * torrent){
        std::cout << "Torrent Added Successfully" << std::endl;

        // wait for torrent plugin to be added before we can go to buy/sell mode...
        QObject::connect(torrent, &joystream::core::Torrent::torrentPluginAdded, [&buyIt, &sellIt, &argv, torrent](const joystream::core::TorrentPlugin *plugin){
            std::cout << "Torrent Plugin Added Successfully" << std::endl;
            std::cout << "Torrent State: " << stateToString(torrent->state()) << std::endl;

            // ready to download?
            if(libtorrent::torrent_status::state_t::downloading == torrent->state()
                    && std::string(argv[1]) == "buy") {
                buyIt(torrent);
                return;
            }

            // ready to upload?
            if(libtorrent::torrent_status::state_t::seeding == torrent->state()
                    && std::string(argv[1]) == "sell") {
                sellIt(torrent);
                return;
            }

            // otherwise.. wait for valid state
            QObject::connect(torrent, &joystream::core::Torrent::stateChanged, [&buyIt, &sellIt, &argv, torrent](libtorrent::torrent_status::state_t state, float progress){
                std::cout << ">>>>>>>>>> Torrent State changed to: " << stateToString(state) << std::endl;

                if(libtorrent::torrent_status::state_t::downloading == state
                        && std::string(argv[1]) == "buy") {
                    buyIt(torrent);
                    return;
                }

                if(libtorrent::torrent_status::state_t::seeding == state
                        && std::string(argv[1]) == "sell") {
                    sellIt(torrent);
                    return;
                }
            });

        });
    });

    if(torrentIdentifier) {
        std::cout << "Adding Torrent" << std::endl;

        auto savePath = (dataDirectory + QDir::separator() + "downloads").toStdString();
        // if torrent is added paused, it doesn't get unpaused in torrentPlugin::start()
        kit->node()->addTorrent(0, 0, "test", std::vector<char>(), savePath, false, *torrentIdentifier,
                               [](libtorrent::error_code &ecode, libtorrent::torrent_handle &th){

            if(ecode) {
                std::cerr << "addTorrent failed: " << ecode.message().c_str() << std::endl;
            }

            std::cout << "Torrent Starting Status:" << stateToString(th.status().state) << std::endl;

        });
    }

    kit->start();

    std::cout << "Starting Qt Application Event loop\n";
    int ret = app.exec();

    std::cout << "Exited Qt Application event loop with code: " << ret << std::endl;
    return ret;
}
