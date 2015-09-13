/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */
#include <common/BitcoinRepresentation.hpp>
#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDir>
#include <QNetworkAccessManager>

#include <core/Config.hpp>
#include <core/controller/ControllerTracker.hpp>
#include <core/controller/Controller.hpp>
#include <core/logger/LoggerManager.hpp>
#include <core/extension/BuyerTorrentPlugin.hpp> // for configurations
#include <core/extension/SellerTorrentPlugin.hpp> // for configurations
#include <core/extension/PluginMode.hpp>
#include <common/Bitcoin.hpp> // defines STANDARD_NUM_SATOSHIES_PER_KB_IN_TX_FEE
#include <common/UnspentP2PKHOutput.hpp>
#include <common/typesafeOutPoint.hpp>

#include <common/Seed.hpp>
#include <gui/MainWindow.hpp>
#include <wallet/Manager.hpp>

//#include <logger/logger.h>

#include <libtorrent/torrent_info.hpp>
#include <libtorrent/error_code.hpp>

// Torrent file
#define RRB_TORRENT "/home/bedeho/Downloads/The.Rise.and.Rise.of.Bitcoin.2014.720p.HDrip.x264.AAC.MVGroup.org.mp4.torrent"
#define LUBUNTU_TORRENT "/home/bedeho/Downloads/Lubuntu 14.04.3 LTS Desktop amd64 CD.torrent"

#define TORRENT_FILE RRB_TORRENT

//#define WALLET_LOCATION "C:/WALLETS/"
#define WALLET_LOCATION "/home/bedeho/JoyStream/wallets/"
#define BLOCKSTORE_LOCATION "/home/bedeho/JoyStream/block_store/"

//#define SAVE_LOCATION "C:/SAVE_OUTPUT/"
#define SAVE_LOCATION "/home/bedeho/JoyStream/save_location/"

// Network to use
#define BITCOIN_NETWORK Coin::Network::testnet3

/**
 * TEMPORARY-TEMPORARY-TEMPORARY-TEMPORARY
 */

#include <CoinCore/typedefs.h> // bytes_t
#include <CoinCore/CoinNodeData.h> // Coin::CoinBlockHeader, Transaction types: outpoints, transactions, ...
#include <CoinCore/BloomFilter.h>

//#include <CoinQ/CoinQ_script.h> // getAddressForTxOutScript
//#include <CoinQ/CoinQ_blocks.h> // CoinQBlockTreeMem
//#include <CoinQ/CoinQ_netsync.h>

// global counters used to pick correct dns and wallet seed for given session
int wallet_seed_counter = 0;
int dns_seed_counter = 0;

// Forward declarations
libtorrent::torrent_info load_torrent(const char * path);

Controller::Torrent::Configuration create_torrent_configuration(libtorrent::torrent_info & torrentInfo, const QString & name);

Controller * create_controller(Controller::Configuration controllerConfiguration,
                               QNetworkAccessManager * manager,
                               bool show_gui,
                               bool use_stdout_logg,
                               libtorrent::torrent_info & torrentInfo,
                               const QString & name);

void add_buyers_with_plugin(Controller::Configuration controllerConfiguration,
                            QNetworkAccessManager * manager,
                            ControllerTracker & controllerTracker,
                            bool show_gui,
                            bool use_stdout_logg,
                            libtorrent::torrent_info & torrentInfo,
                            const QVector<BuyerTorrentPlugin::Configuration> & configurations);

void add_sellers_with_plugin(Controller::Configuration controllerConfiguration,
                             QNetworkAccessManager * manager,
                             ControllerTracker & controllerTracker,
                             bool show_gui,
                             bool use_stdout_logg,
                             libtorrent::torrent_info & torrentInfo,
                             const QVector<SellerTorrentPlugin::Configuration> & configurations);




// JoyStream entry point
int main(int argc, char* argv[]) {

    // Create Qt application: all objects created after this point are owned by this thread
    QApplication app(argc, argv);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(QString::number(JOYSTREAM_VERSION_MAJOR) + "." + QString::number(JOYSTREAM_VERSION_MINOR));

    // Setup command line parsing
    QCommandLineParser parser;
    parser.setApplicationDescription(APPLICATION_DESCRIPTION);

    // Add options
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption showNoUpdateOption("n", "Do not run update manager.");
    parser.addOption(showNoUpdateOption);
    QCommandLineOption showConsoleModeOption("c", "Run in console mode.");
    parser.addOption(showConsoleModeOption);
    QCommandLineOption showFreshOption("f", "Create and use a fresh parameter file.");
    parser.addOption(showFreshOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    // Call update manager, if allowed
    if(!parser.isSet(showNoUpdateOption)) {

        /**
        bool doRestart = updateManager();

        if(doRestart) {
            // Call update runner
        }
        */

    }

    // Check console flag
    bool showView = false;
    if(!parser.isSet(showConsoleModeOption))
        showView = true;

    // Load default state
    Controller::Configuration controllerConfiguration;

    // If fresh flag is not passed,
    // then open existing parameter file
    if(!parser.isSet(showFreshOption)) {

        // Get name of file name
        QString file = QDir::current().absolutePath () + QDir::separator() + PARAMETER_FILE_NAME;
        std::string fileString = file.toStdString();

        // Check that file exists, and that it actually is a file
        if(!QFile::exists(file)) {

            qDebug() << "WARNING: parameter file"
                     << fileString.c_str()
                     << "does not exist. Try fresh run option if problem persists";

            exit(EXIT_FAILURE);

        } else // Load state from file
            controllerConfiguration = Controller::Configuration(fileString.c_str());
    }

    // Network access manager instance used by all code trying to use network
    QNetworkAccessManager manager;

    // Create a controller tracker
    ControllerTracker controllerTracker;

    /**
     * SCENARIO: Downloading & streaming
     * =================================

    // Load torrent
    libtorrent::torrent_info torrentInfo = load_torrent(TORRENT_FILE);

    // Buyers
    Controller * loneBuyer = create_controller(controllerConfiguration,
                                                manager,
                                                true,
                                                true,
                                                torrentInfo,
                                                QString("lone_buyer"),
                                                &PREDETERMINED_SEED_ID);

    controllerTracker.addClient(loneBuyer);

    // Sellers
    add_sellers_with_plugin(controllerConfiguration,
                            manager,
                            controllerTracker,
                            false,
                            true,
                            torrentInfo,
                            QVector<SellerTorrentPlugin::Configuration>()

                            << SellerTorrentPlugin::Configuration(false,
                                                                  10, // Minimum piece price (satoshi)
                                                                  2*3600, // Minimum lock time on refund (seconds)
                                                                  BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.01).satoshies(), // Min fee per kB (satoshi)
                                                                  1, // Max #seller
                                                                  17*60) // Maximum contract confirmation delay (seconds),
                            &PREDETERMINED_SEED_ID
                            );

    */

    /**
     * Paid uploading
     * =================================
     */

    // Load torrent
    libtorrent::torrent_info torrentInfo = load_torrent(TORRENT_FILE);

    // Sellers
    Controller * loneSeller = create_controller(controllerConfiguration,
                                                &manager,
                                                true,
                                                true,
                                                torrentInfo,
                                                QString("lone_seller"));

    controllerTracker.addClient(loneSeller);

    /**
    // TEMPORARY AUTO
    loneSeller->addTorrent(create_torrent_configuration(torrentInfo, "Ubuntu"));

    Controller * loneSeller2 = create_controller(controllerConfiguration,
                                                &manager,
                                                true,
                                                true,
                                                torrentInfo,
                                                QString("lone_seller2"));

    controllerTracker.addClient(loneSeller2);
    */

    // Buyers

    add_buyers_with_plugin(controllerConfiguration, &manager, controllerTracker, true, true, torrentInfo,
                           QVector<BuyerTorrentPlugin::Configuration>()
                           << BuyerTorrentPlugin::Configuration(false,
                                                                178, // Maximum piece price (satoshi)
                                                                4*3600, // Maximum lock time on refund (seconds)
                                                                10000, // BitCoinRepresentation NOT WORKING FOR WHATEVER REASON: BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.1).satoshies(), // Max fee per kB (satoshi)
                                                                1) // #sellers

//                           << BuyerTorrentPlugin::Configuration(false,
//                                                                88, // Maximum piece price (satoshi)
//                                                                5*3600, // Maximum lock time on refund (seconds)
//                                                                10000, // BitCoinRepresentationNOT WORKING FOR WHATEVER REASON: BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.1).satoshies(), // Max fee per kB (satoshi)
//                                                                1) // #sellers

                           );

    /**
     * Run
     */

    // Start event loop: this is the only Qt event loop in the entire application
    app.exec();

    qDebug() << "Application event loop exited, application closing.";

    return 0;
}

libtorrent::torrent_info load_torrent(const char * path) {

    libtorrent::error_code ec;
    libtorrent::torrent_info torrentInfo(path, ec);

    if(ec) {
        qDebug() << "Invalid torrent file 1: " << ec.message().c_str();
        throw std::runtime_error("Could not load torrent file");
    }

    return torrentInfo;
}

Controller::Torrent::Configuration create_torrent_configuration(libtorrent::torrent_info & torrentInfo, const QString & name) {

    return Controller::Torrent::Configuration(torrentInfo.info_hash()
                                              ,torrentInfo.name()
                                              ,(QString(SAVE_LOCATION) + name).toStdString()
                                              ,std::vector<char>()
                                              ,libtorrent::add_torrent_params::flag_update_subscribe
                                              //+libtorrent::add_torrent_params::flag_auto_managed
                                              ,&torrentInfo);
}

Controller * create_controller(Controller::Configuration controllerConfiguration,
                               QNetworkAccessManager * manager,
                               bool show_gui,
                               bool use_stdout_logg,
                               libtorrent::torrent_info & torrentInfo,
                               const QString & name) {

    // Create logging category
    QLoggingCategory * category = global_log_manager.createLogger(name, use_stdout_logg, false);

    // Load wallet
    QString walletFile = QString(WALLET_LOCATION) + name + QString("_wallet.sqlite3");

    // Create wallet if it does not exist
    if(!QFile(walletFile).exists()) {

        qDebug() << "Creating a fresh wallet " << walletFile;

        // Get predefined seed
        Coin::Seed seed = Coin::Seed::testSeeds[wallet_seed_counter];
        Q_ASSERT(wallet_seed_counter <= Coin::Seed::testSeeds.size());

        // Create wallet
        Wallet::Manager::createNewWallet(walletFile, BITCOIN_NETWORK, seed);
    }

    // WE MUST INCREMENT THIS REGARDLESS OF WHETHER A NEW WALLET IS ACTUALLY CREATED
    // BECAUSE A GIVEN CALL SHOULD ALWAYS GIVE THE SAME SEED!
    wallet_seed_counter++;

    // Load wallet
    Wallet::Manager * wallet = new Wallet::Manager(walletFile);

    // Initiliaze
    wallet->BLOCKCYPHER_init(manager);

    // Start spv client
    /**
    QString host = Wallet::Manager::dnsSeeds(BITCOIN_NETWORK)[dns_seed_counter++];
    Q_ASSERT(dns_seed_counter <= Wallet::Manager::dnsSeeds(BITCOIN_NETWORK).size());

    QString blockHeaderStore = QString(BLOCKSTORE_LOCATION) + name + QString(nameFromNetwork(BITCOIN_NETWORK)) + QString("_blockstore.txt");
    //wallet->startSPVClient(blockHeaderStore, host);
    */

    // Create controller: Dangling, but we don't care
    Controller * controller = new Controller(controllerConfiguration, wallet, manager, *category);

    if(show_gui) {
        MainWindow * view = new MainWindow(controller, wallet);
        view->show();
    }

    return controller;
}

void add_buyers_with_plugin(Controller::Configuration controllerConfiguration,
                            QNetworkAccessManager * manager,
                            ControllerTracker & controllerTracker,
                            bool show_gui,
                            bool use_stdout_logg,
                            libtorrent::torrent_info & torrentInfo,
                            const QVector<BuyerTorrentPlugin::Configuration> & configurations) {

    for(int i = 0;i < configurations.size();i++) {

        // Create controller name
        QString name = QString("buyer_") + QString::number(i+1);

        // Create controller and torrent configuration
        Controller * controller = create_controller(controllerConfiguration, manager, show_gui, use_stdout_logg, torrentInfo, name);

        // Create torrent configuration
        Controller::Torrent::Configuration torrentConfiguration = create_torrent_configuration(torrentInfo, name);

        // Grab configuration
        const BuyerTorrentPlugin::Configuration & pluginConfiguration = configurations[i];

        // Amount needed to fund contract (satoshies)
        quint64 minFunds = Payor::minimalFunds(torrentInfo.num_pieces(), pluginConfiguration.maxPrice(), pluginConfiguration.numberOfSellers(), pluginConfiguration.maxFeePerKb());

        // Get funding output - this has to be grabbed from wallet/chain later
        Coin::UnspentP2PKHOutput utxo; // = controller->wallet().getUtxo(minFunds, 1);

        // Check that an utxo was indeed found
        if(utxo.value() == 0) {

            qDebug() << "No utxo found with value no less than:" << minFunds;

            //throw std::exception("No utxo found with sufficient funds.");

            controller->addTorrent(torrentConfiguration);

        } else {

            // Add to controller
            controller->addTorrent(torrentConfiguration, pluginConfiguration, utxo);

        }

        // Track controller
        controllerTracker.addClient(controller);
    }
}

void add_sellers_with_plugin(Controller::Configuration controllerConfiguration, QNetworkAccessManager * manager, ControllerTracker & controllerTracker,
                             bool show_gui, bool use_stdout_logg, libtorrent::torrent_info & torrentInfo,
                             const QVector<SellerTorrentPlugin::Configuration> & configurations) {

    for(int i = 0;i < configurations.size();i++) {

        // Create name
        QString name = QString("seller_") + QString::number(i+1);

        /**
        // Show view
        if(showView)
            _view.show();

        // Update wallet balance in ui
        _view.updateWalletBalance(_wallet->lastComputedZeroConfBalance());
        */

        // Create controller and torrent configuration
        Controller * controller = create_controller(controllerConfiguration, manager, show_gui, use_stdout_logg, torrentInfo, name);

        // Create torrent configuration
        Controller::Torrent::Configuration torrentConfiguration = create_torrent_configuration(torrentInfo, name);

        // Grab configuration
        const SellerTorrentPlugin::Configuration & pluginConfiguration = configurations[i];

        // Add to controller
        controller->addTorrent(torrentConfiguration, pluginConfiguration);

        // Track controller
        controllerTracker.addClient(controller);
    }
}
