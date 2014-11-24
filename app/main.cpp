#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDir>

#include <iostream>

#include "lib/Config.hpp"
#include "lib/ControllerTracker.hpp"
#include "lib/logger/LoggerManager.hpp"

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

    // Create a tracker
    ControllerTracker tracker;

    // Create category
    QLoggingCategory * mainCategory = global_log_manager.createLogger("main", false, true);
    //QLoggingCategory * peerCategory = global_log_manager.createLogger("peer", false, false);

    // Create main client
    Controller client(controllerState, true, mainCategory);
    tracker.addClient(&client);
    std::cout << "Started main client." << std::endl;

    /*
    // Create peer client
    Controller pclient(controllerState, false, peerCategory);
    tracker.addClient(&pclient);
    std::cout << "Started peer client." << std::endl;
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
