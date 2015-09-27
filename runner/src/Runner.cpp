/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 23 2015
 */

#include <runner/Runner.hpp>
#include <runner/ControllerBarrier.hpp>
#include <core/logger/LoggerManager.hpp>
#include <common/Network.hpp>
#include <common/Seed.hpp>
#include <gui/MainWindow.hpp>
#include <wallet/Manager.hpp>

#include <QNetworkAccessManager>

namespace Runner {

    libtorrent::torrent_info load_torrent(const char * path) {

        libtorrent::error_code ec;
        //libtorrent::torrent_info torrentInfo = new libtorrent::torrent_info(path, ec);
        libtorrent::torrent_info torrentInfo(path, ec);

        if(ec) {
            qDebug() << "Invalid torrent file: " << ec.message().c_str();
            throw std::runtime_error("Could not load torrent file");
        } else
            qDebug() << "Loaded torrent file:" << path;

        return torrentInfo;
    }

    Controller::Torrent::Configuration create_torrent_configuration(const QString & torrentFilePath, const QString & savePath) {

        std::string str = torrentFilePath.toStdString();

        boost::intrusive_ptr<libtorrent::torrent_info> torrentFile(new libtorrent::torrent_info(load_torrent(str.c_str())));

        return Controller::Torrent::Configuration(torrentFile->info_hash(),
                                                  torrentFile->name(),
                                                  savePath.toStdString(),
                                                  std::vector<char>(),
                                                  libtorrent::add_torrent_params::flag_update_subscribe, //+libtorrent::add_torrent_params::flag_auto_managed
                                                  torrentFile);
    }

    Controller * create_client(const Controller::Configuration & configuration,
                               bool show_gui,
                               bool use_stdout_logg,
                               const QString & home,
                               const QString & name,
                               const Coin::Seed & seed,
                               Coin::Network network,
                               QNetworkAccessManager * manager,
                               const QString & BlockcypherToken) {
        // Create logging category
        QLoggingCategory * category = global_log_manager.createLogger(name, use_stdout_logg, false);

        // Create client folder if it does not alreadye exist
        QDir dir(home);

        if(!dir.exists())
            throw std::runtime_error("Home folder does not exist:" + home.toStdString());

        qDebug((*category)) << "Creating client" << name << " in folder" << home;

        dir.mkdir(name);
        dir.cd(name);

        // If torrent data directory exists, delete it, and create new one
        if(dir.cd("torrent_data")) {
            dir.removeRecursively();
            dir.cdUp();
        }
        dir.mkdir("torrent_data");

        // Create/load wallet
        QString walletFile = dir.path() + QDir::separator() + QString("wallet.sqlite3");

        // Create new wallet if it does not exist
        if(!QFile(walletFile).exists()) {

            qDebug((*category)) << "Creating a fresh wallet " << walletFile;

            // Create wallet
            Wallet::Manager::createNewWallet(walletFile, network, seed);
        }

        // Load wallet
        Wallet::Manager * wallet = new Wallet::Manager(walletFile);

        // Initiliaze
        wallet->BLOCKCYPHER_init(manager, BlockcypherToken);

        // Create controller: Dangling, but we don't care
        Controller * controller = new Controller(configuration, wallet, manager, *category);

        if(show_gui) {
            MainWindow * view = new MainWindow(controller, wallet, "-[" + name + "]");
            view->show();
        }

        return controller;
    }

    void add_sellers_with_plugin(const Controller::Configuration & configuration,
                                 Runner::ControllerBarrier & barrier,
                                 bool show_gui,
                                 bool use_stdout_logg,
                                 const QString & home,
                                 const Controller::Torrent::Configuration & torrentConfiguration,
                                 const QVector<SellerTorrentPlugin::Configuration> & configurations,
                                 const QVector<Coin::Seed> seeds,
                                 Coin::Network network,
                                 QNetworkAccessManager * manager,
                                 const QString & BlockcypherToken) {

        Q_ASSERT(configurations.size() <= seeds.size());

        for(int i = 0;i < configurations.size();i++) {

            // Create name
            QString name = QString("seller_") + QString::number(i+1);

            // Create controller and torrent configuration
            Controller * controller = create_client(configuration,
                                                    show_gui,
                                                    use_stdout_logg,
                                                    home,
                                                    name,
                                                    seeds[i],
                                                    network,
                                                    manager,
                                                    BlockcypherToken);

            /**
             * WHEN LOADING DATA FROM CLIENT DIRECTORY
             *
            // Go into home directory of client
            QDir dir(home);
            dir.cd(name);

            // Create torrent directory, in case this is first time, and go into it
            dir.mkdir("torrent_data");
            dir.cd("torrent_data");

            // Check that the torrent data file is there
            QFileInfo torrentFileInfo(dir, QString::fromStdString(torrentInfo.name()));
            if(!torrentFileInfo.exists())
                throw std::runtime_error("Torrent file not found " + torrentFileInfo.path().toStdString());

            // Create torrent configuration
            Controller::Torrent::Configuration torrentConfiguration = create_torrent_configuration(torrentInfo, dir.path());

            // Create torrent configuration
            Controller::Torrent::Configuration torrentConfiguration = create_torrent_configuration(torrentInfo, dir.path());
            */

            // Add to controller
            controller->addTorrent(torrentConfiguration, configurations[i]);

            // Track controller
            barrier.add(controller);
        }
    }

    void add_buyers_with_plugin(const Controller::Configuration & configuration,
                                ControllerBarrier & barrier,
                                bool show_gui,
                                bool use_stdout_logg,
                                const QString & home,
                                libtorrent::torrent_info & torrentInfo,
                                const QVector<BuyerTorrentPlugin::Configuration> & configurations,
                                const QVector<Coin::Seed> & seeds,
                                Coin::Network network,
                                QNetworkAccessManager * manager,
                                const QString & BlockcypherToken) {

        Q_ASSERT(configurations.size() <= seeds.size());

        for(int i = 0;i < configurations.size();i++) {

            // Create controller name
            QString name = QString("buyer_") + QString::number(i+1);

            // Create controller and torrent configuration
            Controller * controller = create_client(configuration,
                                                    show_gui,
                                                    use_stdout_logg,
                                                    home,
                                                    name,
                                                    seeds[i],
                                                    network,
                                                    manager,
                                                    BlockcypherToken);


            // Create torrent configuration
            Controller::Torrent::Configuration torrentConfiguration;// = create_torrent_configuration(torrentInfo, dir.path());

            // Try to fund plugin, and if it works
            // add torrent with given plugin to controller,
            // otherwise nothing is added
            try {

                /**
                BuyerTorrentPlugin::Configuration pluginConfiguration = configurations[i];

                controller->addTorrent(torrentConfiguration, pluginConfiguration, find_utxo(torrentInfo.num_pieces(),
                                                                                            pluginConfiguration,
                                                                                           controller->wallet()));
                */

            } catch (const std::runtime_error & e) {
                qDebug() << "Torrent not added";
            }

            // Track controller
            barrier.add(controller);
        }
    }

    Coin::UnspentP2PKHOutput find_utxo(int num_pieces,
                                       const BuyerTorrentPlugin::Configuration & pluginConfiguration,
                                       Wallet::Manager * wallet) {

        // Amount needed to fund contract (satoshies)
        quint64 minFunds = Payor::minimalFunds(num_pieces,
                                               pluginConfiguration.maxPrice(),
                                               pluginConfiguration.numberOfSellers(),
                                               pluginConfiguration.maxFeePerKb());

        // Get funding output - this has to be grabbed from wallet/chain later
        //Coin::UnspentP2PKHOutput utxo; // = controller->wallet().getUtxo(minFunds, 1);
        Coin::UnspentP2PKHOutput utxo = wallet->BLOCKCYPHER_lock_one_utxo(minFunds);

        // Check that an utxo was indeed found
        if(utxo.value() == 0) {
            qDebug() << "No *single* utxo found enough value to cover:" << minFunds;
            throw std::runtime_error("No utxo found with sufficient funds.");
        }

        return utxo;
    }

}
