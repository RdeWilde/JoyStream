#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDir>
#include <QNetworkAccessManager>

#include <lib/Config.hpp>
#include <lib/ControllerTracker.hpp>
#include <lib/controller/Controller.hpp>
#include <lib/extension/BuyerTorrentPlugin.hpp> // for configurations
#include <lib/extension/SellerTorrentPlugin.hpp> // for configurations
#include <lib/logger/LoggerManager.hpp>
#include <lib/extension/PluginMode.hpp>
#include <lib/extension/BitCoin/BitCoin.hpp> // defines STANDARD_NUM_SATOSHIES_PER_KB_IN_TX_FEE

#include <libtorrent/torrent_info.hpp>
#include <libtorrent/error_code.hpp>

#ifndef Q_MOC_RUN
#include <boost/intrusive_ptr.hpp>
#endif Q_MOC_RUN

// Forward declarations
bool updateManager();

// BitSwapr entry point
void main(int argc, char* argv[]) {

    // Create Qt application: all objects created after this point are owned by this thread
    QApplication app(argc, argv);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion(QString::number(BITSWAPR_VERSION_MAJOR) + "." + QString::number(BITSWAPR_VERSION_MINOR));

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

        bool doRestart = updateManager();

        if(doRestart) {
            // Call update runner
        }
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

    /**
     * Load torrent ================================================
     */

    const char * torrent = "C:/TORRENTS/RRB.torrent";
    //const char * torrent  = "C:/TORRENTS/Aint No Love Crucified.mp3.torrent";

    libtorrent::error_code ec;
    libtorrent::torrent_info torrentInfo(torrent, ec);

    if(ec) {
        qDebug() << "Invalid torrent file 1: " << ec.message().c_str();
        return;
    }

    // Create a controller tracker
    ControllerTracker controllerTracker;

    /**
     * Experiment paramters ========================================
     */

    // Number of Buyers
    int buyer_count = 1;
    bool buyer_show_gui = true;
    bool buyer_use_stdout_logg = buyer_show_gui;

    // Number of sellers
    int seller_count = 1;
    bool seller_show_gui = true;
    bool seller_use_stdout_logg = seller_show_gui;

    /**
     * Buyer =======================================================
     */

    QVector<Controller *> buyer_controllers(buyer_count);
    QVector<std::string> buyer_loggNames(buyer_count);
    QVector<std::string> buyer_walletFileName(buyer_count);
    QVector<std::string> buyer_outPutDir(buyer_count);

    for(int i = 0;i < buyer_count;i++) {

        // Create logging category: med logging til skjerm
        buyer_loggNames[i] = (std::string("buyer_") + std::to_string(i+1));
        QLoggingCategory * buyerCategory = global_log_manager.createLogger(buyer_loggNames[i].c_str(), buyer_use_stdout_logg, false);

        // Create wallet name
        buyer_walletFileName[i] = (std::string("c:/WALLETS/buyer_wallet") + std::to_string(i+1) + std::string(".dat"));
        controllerConfiguration.setWalletFile(buyer_walletFileName[i].c_str());

        // Create main client
        Controller * buyerClient = new Controller(controllerConfiguration,
                                                  buyer_show_gui,
                                                  manager,
                                                  "Faucet http://faucet.xeno-genesis.com/",
                                                  *buyerCategory);
        buyer_controllers[i] = buyerClient;

        // Create output directory name
        buyer_outPutDir[i] = (std::string("C:/SAVE_OUTPUT/buyer_") + std::to_string(i+1));

        // Create buyer torrent configuration
        Controller::Torrent::Configuration buyerTorrentConfiguration(torrentInfo.info_hash()
                                                      ,torrentInfo.name()
                                                      ,buyer_outPutDir[i]
                                                      ,std::vector<char>()
                                                      ,libtorrent::add_torrent_params::flag_update_subscribe
                                                      //+libtorrent::add_torrent_params::flag_auto_managed
                                                      ,&torrentInfo);

        // Maximum piece price (satoshi)
        quint64 maxPrice = 300;

        // Maximum lock time on refund
        int maxLockTime = 5*3600;

        // Max fee per kB (satoshi)
        int maxFeePerkB = 1*static_cast<int>(STANDARD_NUM_SATOSHIES_PER_KB_IN_TX_FEE);

        // Amount needed to fund contract (satoshies)
        quint64 minFunds = Payor::minimalFunds(torrentInfo.num_pieces(),maxPrice, seller_count, maxFeePerkB);

        // Synch wallet
        buyerClient->wallet().synchronize();

        // Get funding output - this has to be grabbed from wallet/chain later
        UnspentP2PKHOutput utxo = buyerClient->wallet().getUtxo(minFunds, 1);

        // Create configuration
        BuyerTorrentPlugin::Configuration configuration(false,
                                                        maxPrice,
                                                        maxLockTime,
                                                        maxFeePerkB,
                                                        seller_count);

        // Check that an utxo was indeed found
        if(utxo.value() == 0) {
            qDebug() << "No utxo found with value no less than:" << minFunds;

            buyerClient->addTorrent(buyerTorrentConfiguration);
        } else {
            // Add to client
            buyerClient->addTorrent(buyerTorrentConfiguration, configuration, utxo);
        }

        // Track controller
        controllerTracker.addClient(buyerClient);
    }

    /**
     * Seller =======================================================
    */

    QVector<Controller *> seller_controllers(seller_count);
    QVector<std::string> seller_loggNames(seller_count);
    QVector<std::string> seller_walletFileName(seller_count);
    QVector<std::string> seller_outPutDir(seller_count);

    for(int i = 0;i < seller_count;i++) {

        // Create logging category: uten logging til skjerm
        seller_loggNames[i] = (std::string("seller_") + std::to_string(i+1));
        QLoggingCategory * sellerCategory = global_log_manager.createLogger(seller_loggNames[i].c_str(), seller_use_stdout_logg, false);

        // Create wallet name
        seller_walletFileName[i] = (std::string("c:/WALLETS/seller_wallet_") + std::to_string(i+1) + std::string(".dat"));
        controllerConfiguration.setWalletFile(seller_walletFileName[i].c_str());

        // Create peer client
        Controller * sellerClient = new Controller(controllerConfiguration,
                                                   seller_show_gui,
                                                   manager,
                                                   "Faucet http://faucet.xeno-genesis.com/",
                                                   *sellerCategory);
        seller_controllers[i] = sellerClient;

        // Create output directory name
        seller_outPutDir[i] = (std::string("C:/SAVE_OUTPUT/seller_") + std::to_string(i+1));

        // Create torrent configuration
        Controller::Torrent::Configuration sellerTorrentConfiguration(torrentInfo.info_hash()
                                                      ,torrentInfo.name()
                                                      ,seller_outPutDir[i]
                                                      ,std::vector<char>()
                                                      ,libtorrent::add_torrent_params::flag_update_subscribe
                                                      ,&torrentInfo);

        // Min fee per kB (satoshi)
        int minFeePerkB = static_cast<int>(STANDARD_NUM_SATOSHIES_PER_KB_IN_TX_FEE);

        // Maximum piece price (satoshi)
        quint64 minPrice = 20;

        // Minimum lock time on refund
        int minLockTime = 2*3600;

        // Maximum contract confirmation delay
        quint32 maxContractConfirmationDelay = 17*60;

        // Create seller torrent plugin configuration
        SellerTorrentPlugin::Configuration SellerTorrentPluginConfiguration(false,
                                                                            minPrice,
                                                                            minLockTime,
                                                                            minFeePerkB,
                                                                            seller_count,
                                                                            maxContractConfirmationDelay);

        // Add to client
        sellerClient->addTorrent(sellerTorrentConfiguration, SellerTorrentPluginConfiguration);

        // Track controller
        controllerTracker.addClient(sellerClient);

    }

    // Start event loop: this is the only Qt event loop in the entire application
    app.exec();

    qDebug() << "Application event loop exited, application closing.";
}

bool updateManager() {

    // If we have just downloaded a new binary, then run it and then quit

    // Query blockchain or most recent location

    // Connect to location and check what version is there

    // If its newer, download it to particular location

    // then return to restart:

    return true;
}
