#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDir>
#include <QNetworkAccessManager>

#include <iostream>

#include "lib/Config.hpp"
#include "lib/ControllerTracker.hpp"
#include "lib/controller/Controller.hpp"
#include "lib/extension/BuyerTorrentPlugin.hpp" // for configurations
#include "lib/extension/SellerTorrentPlugin.hpp" // for configurations
#include "lib/logger/LoggerManager.hpp"

//#include "lib/controller/ControllerConfiguration.hpp"
//#include "lib/controller/TorrentConfiguration.hpp"
//#include "lib/extension/TorrentPluginConfiguration.hpp"

#include "lib/extension/PluginMode.hpp"

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

            std::cerr << "WARNING: parameter file "
                      << fileString.c_str()
                      << " does not exist." << std::endl
                      << "Try fresh run option if problem persists" << std::endl << std::endl;

            exit(EXIT_FAILURE);

        } else // Load state from file
            controllerConfiguration = Controller::Configuration(fileString.c_str());
    }

    // Network access manager instance used by all code trying to use network
    QNetworkAccessManager manager;

    /**
     * Load torrent ================================================
     */

    // VUZE-test.mp4.torrent: Rise and Rise of BitCoin
    const char * torrent  = "C:/Users/Sindre/Desktop/TORRENTS/VUZE-test.mp4.torrent";

    libtorrent::error_code ec;
    libtorrent::torrent_info torrentInfo(torrent, ec);

    if(ec) {
        std::cerr << "Invalid torrent file 1: " << ec.message().c_str();
        return;
    }

    // Create a controller tracker
    ControllerTracker controllerTracker;

    /**
     * Buyer =======================================================
     */

    // Create logging category: med logging til skjerm
    QLoggingCategory * buyerCategory = global_log_manager.createLogger("buyer", true, false);

    // Create main client
    controllerConfiguration.setWalletFile("C:/Users/Sindre/Desktop/BUILD_DEBUG/app/debug/buyer_wallet.dat");
    Controller buyerClient(controllerConfiguration, true, manager, "Faucet http://faucet.xeno-genesis.com/", *buyerCategory);

    std::cout << "Started buyer client." << std::endl;

    // Create buyer torrent plugin configuration
    Controller::Torrent::Configuration buyerTorrentConfiguration(torrentInfo.info_hash()
                                                  ,torrentInfo.name()
                                                  ,std::string("C:/Users/Sindre/Desktop/SAVE_OUTPUT/MAIN")
                                                  ,std::vector<char>()
                                                  ,libtorrent::add_torrent_params::flag_update_subscribe
                                                  //+libtorrent::add_torrent_params::flag_auto_managed
                                                  ,&torrentInfo);

    // Add to client
    buyerClient.addTorrent(buyerTorrentConfiguration);

    // Track controller
    controllerTracker.addClient(&buyerClient);



    /**
     * Seller =======================================================
     */

    // Create logging category: uten logging til skjerm
    QLoggingCategory * sellerCategory = global_log_manager.createLogger("seller", true, false); // ("seller", false, false)

    // Create peer client
    controllerConfiguration.setWalletFile("C:/Users/Sindre/Desktop/BUILD_DEBUG/app/debug/seller_wallet.dat");
    Controller sellerClient(controllerConfiguration, true, manager, "Faucet http://faucet.xeno-genesis.com/", *sellerCategory);

    std::cout << "Started seller client." << std::endl;

    // Create torrent configuration
    Controller::Torrent::Configuration sellerTorrentConfiguration(torrentInfo.info_hash()
                                                  ,torrentInfo.name()
                                                  ,std::string("C:/Users/Sindre/Desktop/SAVE_OUTPUT/PEER")
                                                  ,std::vector<char>()
                                                  ,libtorrent::add_torrent_params::flag_update_subscribe
                                                  ,&torrentInfo);

    // Create seller torrent plugin configuration
    SellerTorrentPlugin::Configuration SellerTorrentPluginConfiguration(false,
                                                                        10, // 10 satoshies per piece!
                                                                        60*60,// 1h maximum lock time
                                                                        0* 100000, // minfeeperbyte
                                                                        1,
                                                                        30); // maximum confirmation delay

    // Add to client
    sellerClient.addTorrent(sellerTorrentConfiguration, SellerTorrentPluginConfiguration);

    // Track controller
    controllerTracker.addClient(&sellerClient);

    // Start event loop: this is the only Qt event loop in the entire application
    app.exec();

    std::cout << "Application event loop exited, application closing." << std::endl;
}

bool updateManager() {

    // If we have just downloaded a new binary, then run it and then quit

    // Query blockchain or most recent location

    // Connect to location and check what version is there

    // If its newer, download it to particular location

    // then return to restart:

    return true;
}
