#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDir>

#include <iostream>

#include "lib/Config.hpp"
#include "lib/ControllerTracker.hpp"
#include "lib/controller/Controller.hpp"
#include "lib/logger/LoggerManager.hpp"

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
    ControllerState controllerState;

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
            controllerState = ControllerState(fileString.c_str());
    }

    // Create a controller tracker
    ControllerTracker controllerTracker;

    // Load torrent
    QString torrentFile  = "C:/Users/bedeho/Documents/GitHub/QtBitSwapr/tests/resources/BITSWAPR.TEST.torrent";

    libtorrent::error_code ec;
    boost::intrusive_ptr<libtorrent::torrent_info> torrentInfoPointer = new libtorrent::torrent_info(torrentFile.toStdString().c_str(), ec);

    if(ec) {
        std::cerr << "Invalid torrent file: " << ec.message().c_str();
        return;
    }

    // Create main client
    QLoggingCategory * mainCategory = global_log_manager.createLogger("main", false, true);
    Controller main(controllerState, true, mainCategory);
    controllerTracker.addClient(&main);

    std::cout << "Started main client." << std::endl;

    libtorrent::add_torrent_params paramsMain;
    paramsMain.ti = torrentInfoPointer;
    paramsMain.save_path = "C:/TORRRENT_OUTPUT/TEST/MAIN";
    QMetaObject::invokeMethod(&main, "addTorrent", Q_ARG(libtorrent::add_torrent_params &, paramsMain));

/*
    // Create peer client
    QLoggingCategory * peerCategory = global_log_manager.createLogger("peer", false, false); // false
    Controller peer(controllerState, true, peerCategory);
    controllerTracker.addClient(&peer);

    std::cout << "Started peer client." << std::endl;
    libtorrent::add_torrent_params paramsPeer;
    paramsPeer.ti = torrentInfoPointer;
    paramsPeer.save_path = "C:/TORRRENT_OUTPUT/TEST/PEER";
    QMetaObject::invokeMethod(&peer, "addTorrent", Q_ARG(libtorrent::add_torrent_params &, paramsPeer));
*/

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
