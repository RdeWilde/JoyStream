#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDir>

#include <iostream>

#include "lib/Config.hpp"
#include "lib/ControllerTracker.hpp"
#include "lib/controller/Controller.hpp"
#include "lib/logger/LoggerManager.hpp"
#include "lib/controller/ControllerConfiguration.hpp"
#include "lib/controller/TorrentConfiguration.hpp"
#include "lib/extension/TorrentPluginConfiguration.hpp"

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
    ControllerConfiguration controllerConfiguration;

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
            controllerConfiguration = ControllerConfiguration(fileString.c_str());
    }

    // Torrent file: (UTORRENT-TEST.torrent, VUZE-test.mp4.torrent)
    const char * torrent  = "C:/Users/Sindre/Desktop/TORRENTS/VUZE-test.mp4.torrent";

    /**
     * Main client =======================================================
     */

    // Load torrent
    libtorrent::error_code ec;
    libtorrent::torrent_info mainTorrentInfo(torrent, ec);

    if(ec) {
        std::cerr << "Invalid torrent file 1: " << ec.message().c_str();
        return;
    }

    // Create logging category: med logging til skjerm
    QLoggingCategory * mainCategory = global_log_manager.createLogger("main", true, false);

    // Create main client
    Controller mainClient(controllerConfiguration, true, *mainCategory);

    std::cout << "Started main client." << std::endl;

    // Create configuration
    TorrentConfiguration mainTorrentConfiguration(mainTorrentInfo.info_hash()
                                                  ,mainTorrentInfo.name()
                                                  ,std::string("C:/Users/Sindre/Desktop/SAVE_OUTPUT/MAIN")
                                                  ,std::vector<char>()
                                                  ,0
                                                  ,mainTorrentInfo
                                                  ,TorrentPluginConfiguration(PluginMode::Buyer, true, true));

    // Add to client
    mainClient.addTorrent(mainTorrentConfiguration);

    /**
     * Peer client =======================================================
     */

    // Load torrent
    libtorrent::torrent_info peerTorrentInfo(torrent, ec);

    if(ec) {
        std::cerr << "Invalid torrent file 2: " << ec.message().c_str();
        return;
    }

    // Create logging category: uten logging til skjerm
    QLoggingCategory * peerCategory = global_log_manager.createLogger("peer", false, false);

    // Create peer client
    Controller peerClient(controllerConfiguration, true, *peerCategory);

    std::cout << "Started peer client." << std::endl;

    // Create configuration
    TorrentConfiguration peerTorrentConfiguration(peerTorrentInfo.info_hash()
                                                  ,peerTorrentInfo.name()
                                                  ,std::string("C:/Users/Sindre/Desktop/SAVE_OUTPUT/PEER")
                                                  ,std::vector<char>()
                                                  ,0
                                                  ,peerTorrentInfo
                                                  ,TorrentPluginConfiguration(PluginMode::Seller, true, true));

    // Add to client
    peerClient.addTorrent(peerTorrentConfiguration);

    // Create a controller tracker
    ControllerTracker controllerTracker;
    controllerTracker.addClient(&mainClient);
    controllerTracker.addClient(&peerClient);

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
