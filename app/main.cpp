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

#include <common/BitCoinRepresentation.hpp>

#include <libtorrent/torrent_info.hpp>
#include <libtorrent/error_code.hpp>

#ifndef Q_MOC_RUN
#include <boost/intrusive_ptr.hpp>
#endif Q_MOC_RUN

// Forward declarations
bool updateManager();

libtorrent::torrent_info load_torrent(const char * path);

Controller::Torrent::Configuration create_torrent_configuration(libtorrent::torrent_info & torrentInfo, const QString & name);

Controller * create_controller(Controller::Configuration controllerConfiguration, QNetworkAccessManager & manager,
                                                     bool show_gui, bool use_stdout_logg, libtorrent::torrent_info & torrentInfo, const QString & name);

void add_buyers_with_plugin(Controller::Configuration controllerConfiguration, QNetworkAccessManager & manager, ControllerTracker & controllerTracker,
                            bool show_gui, bool use_stdout_logg, libtorrent::torrent_info & torrentInfo,
                            const QVector<BuyerTorrentPlugin::Configuration> & configurations);

void add_sellers_with_plugin(Controller::Configuration controllerConfiguration, QNetworkAccessManager & manager, ControllerTracker & controllerTracker,
                             bool show_gui, bool use_stdout_logg, libtorrent::torrent_info & torrentInfo,
                             const QVector<SellerTorrentPlugin::Configuration> & configurations);

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

    // Create a controller tracker
    ControllerTracker controllerTracker;

    /**
     * Downloading & streaming
     *

    // Load torrent
    libtorrent::torrent_info torrentInfo = load_torrent("C:/TORRENTS/Rise and Rrise of BitCoin.torrent");

    // Buyers
    Controller * loneBuyer = create_controller(controllerConfiguration, manager, true, true, torrentInfo, QString("lone_buyer"));

    controllerTracker.addClient(loneBuyer);

    // Sellers
    add_sellers_with_plugin(controllerConfiguration, manager, controllerTracker, false , true, torrentInfo,
                            QVector<SellerTorrentPlugin::Configuration>()

                            << SellerTorrentPlugin::Configuration(false,
                                                                  10, // Minimum piece price (satoshi)
                                                                  2*3600, // Minimum lock time on refund (seconds)
                                                                  BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.01).satoshies(), // Min fee per kB (satoshi)
                                                                  1, // Max #seller
                                                                  17*60) // Maximum contract confirmation delay (seconds)
                            );
    */

    /**
     * Paid uploading
     */

    // Load torrent
    libtorrent::torrent_info torrentInfo = load_torrent("C:/TORRENTS/Rise and Rrise of BitCoin.torrent"); // C:/TORRENTS/Aint No Love Crucified.mp3.torrent

    // Buyers
    add_buyers_with_plugin(controllerConfiguration, manager, controllerTracker, true , true, torrentInfo,
                           QVector<BuyerTorrentPlugin::Configuration>()

                           << BuyerTorrentPlugin::Configuration(false,
                                                                178, // Maximum piece price (satoshi)
                                                                4*3600, // Maximum lock time on refund (seconds)
                                                                BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.1).satoshies(), // Max fee per kB (satoshi)
                                                                1) // #sellers
                           << BuyerTorrentPlugin::Configuration(false,
                                                                88, // Maximum piece price (satoshi)
                                                                5*3600, // Maximum lock time on refund (seconds)
                                                                BitCoinRepresentation(BitCoinRepresentation::BitCoinPrefix::Milli, 0.1).satoshies(), // Max fee per kB (satoshi)
                                                                1) // #sellers
                           );

    // Sellers
    Controller * loneSeller = create_controller(controllerConfiguration, manager, true, true, torrentInfo, QString("lone_seller"));
    controllerTracker.addClient(loneSeller);

    /**
     * Run
     */

    // Start event loop: this is the only Qt event loop in the entire application
    app.exec();

    qDebug() << "Application event loop exited, application closing.";
}

libtorrent::torrent_info load_torrent(const char * path) {

    libtorrent::error_code ec;
    libtorrent::torrent_info torrentInfo(path, ec);

    if(ec) {
        qDebug() << "Invalid torrent file 1: " << ec.message().c_str();
        throw std::exception();
    }

    return torrentInfo;
}

Controller::Torrent::Configuration create_torrent_configuration(libtorrent::torrent_info & torrentInfo, const QString & name) {

    return Controller::Torrent::Configuration(torrentInfo.info_hash()
                                              ,torrentInfo.name()
                                              ,(QString("C:/SAVE_OUTPUT/") + name).toStdString()
                                              ,std::vector<char>()
                                              ,libtorrent::add_torrent_params::flag_update_subscribe
                                              //+libtorrent::add_torrent_params::flag_auto_managed
                                              ,&torrentInfo);
}

Controller * create_controller(Controller::Configuration controllerConfiguration, QNetworkAccessManager & manager,
                                                       bool show_gui, bool use_stdout_logg, libtorrent::torrent_info & torrentInfo, const QString & name) {

    // Create logging category
    QLoggingCategory * category = global_log_manager.createLogger(name, use_stdout_logg, false);

    // Create wallet name
    controllerConfiguration.setWalletFile(QString("C:/WALLETS/") + name + QString("_wallet.dat"));

    // Create controller: Dangling, but we don't care
    return new Controller(controllerConfiguration,
                         show_gui,
                         manager,
                         "Faucet http://faucet.xeno-genesis.com/",
                         *category);
}

void add_buyers_with_plugin(Controller::Configuration controllerConfiguration, QNetworkAccessManager & manager, ControllerTracker & controllerTracker,
                            bool show_gui, bool use_stdout_logg, libtorrent::torrent_info & torrentInfo,
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

        // Synch wallet
        controller->wallet().synchronize();

        // Get funding output - this has to be grabbed from wallet/chain later
        UnspentP2PKHOutput utxo = controller->wallet().getUtxo(minFunds, 1);

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

void add_sellers_with_plugin(Controller::Configuration controllerConfiguration, QNetworkAccessManager & manager, ControllerTracker & controllerTracker,
                             bool show_gui, bool use_stdout_logg, libtorrent::torrent_info & torrentInfo,
                             const QVector<SellerTorrentPlugin::Configuration> & configurations) {

    for(int i = 0;i < configurations.size();i++) {

        // Create name
        QString name = QString("seller_") + QString::number(i+1);

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

bool updateManager() {

    // If we have just downloaded a new binary, then run it and then quit

    // Query blockchain or most recent location

    // Connect to location and check what version is there

    // If its newer, download it to particular location

    // then return to restart:

    return true;
}
