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

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        std::cerr << "usage: ./app_kit_cli torrent-file" << std::endl;
        return 1;
    }

    QCoreApplication app(argc, argv);

    std::cout << "Creating AppKit Instance\n";

    joystream::AppKit* kit = joystream::AppKit::createInstance(QDir::homePath(), Coin::Network::testnet3);

    std::vector<Coin::P2PKHAddress> addresses = kit->wallet()->listReceiveAddresses();

    std::string depositAddress;

    if(addresses.size() > 0){
        depositAddress = addresses.front().toBase58CheckEncoding().toStdString();
    } else {
        depositAddress = kit->wallet()->generateReceiveAddress().toBase58CheckEncoding().toStdString();
    }

    std::cout << "Wallet Deposit Address: " <<  depositAddress << std::endl;

    std::cout << "Wallet Balance: " << kit->wallet()->unconfirmedBalance() << std::endl;

    std::cout << "Starting Wallet Sync.. this could take a while while loading blocktree\n";

    kit->syncWallet();

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

    libtorrent::error_code ec;
    auto ti = boost::make_shared<libtorrent::torrent_info>(std::string(argv[1]), boost::ref(ec), 0);
    if (ec)
    {
        std::cerr << ec.message().c_str() << std::endl;
        return 1;
    }

    // process addedTorrent Signal when torrent is added
    QObject::connect(kit->node(), &joystream::core::Node::addedTorrent, [&ti, &kit](const joystream::core::Torrent *torrent){
        std::cout << "Got torrent added signal, waiting for plugin added signal" << std::endl;
        if(torrent->infoHash() != ti->info_hash()) return;

        // wait for torrent pluging to be added before we can go to buy mode...
        QObject::connect(torrent, &joystream::core::Torrent::torrentPluginAdded, [&kit, torrent](const joystream::core::TorrentPlugin *plugin){
            std::cout << "Torrent Plugin Added... tryin to buy torrent" << std::endl;

            Q_ASSERT(plugin);

            kit->buyTorrent(torrent, joystream::protocol_session::BuyingPolicy(), joystream::protocol_wire::BuyerTerms(), [](const std::exception_ptr &eptr){
                std::cerr << "Error Buying Torrent" << std::endl;
            });
        });
    });

    std::cout << "Adding Torrent" << std::endl;

    auto savePath = (QDir::homePath()+"/Downloads/").toStdString();

    kit->node()->addTorrent(0, 0, "test", std::vector<char>(), savePath, false, joystream::core::TorrentIdentifier(ti),
                           [&kit](libtorrent::error_code &ecode, libtorrent::torrent_handle &th){

        if(ecode) {
            std::cerr << "Node::addTorrent() failed: " << ecode.message().c_str() << std::endl;
            return;
        }

        std::cout << "Torrent Added" << std::endl;
    });

    std::cout << "Starting Qt Application Event loop\n";
    int ret = app.exec();

    std::cout << "Exited Qt Application event loop with code: " << ret << std::endl;

    return ret;
}

