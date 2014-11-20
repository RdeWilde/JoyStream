#include <QApplication>
#include <QCommandLineParser>
#include <QString>
#include <QDir>

#include <iostream>

#include "lib/BitSwapr.hpp"
#include "lib/Config.hpp"
#include "lib/logger/LoggerManager.hpp"

// Forward declarations
bool updateManager();

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

    /*
    // Create category
    QLoggingCategory * mainCategory = global_log_manager("main", false, true),
            * peerCategory = global_log_manager("peer", false, false);
    */

    std::cout << "Main thread id = " << QThread::currentThreadId() << std::endl;

    // Create main client
    BitSwapr client(controllerState, showView);
    client.start();
    std::cout << "Started main client thread." << std::endl;

    /*
    // Create peer client
    BitSwapr client(controllerState, showView);
    client.start();
    std::cout << "Started main client thread." << std::endl;
    */

    // Start application event loop,
    // it is responsible for servicing event loop for BitSwapr
    // objects, which among which means connecting runner thread in
    // each object to its runner_entry slot
    app.exec();

    // Notify that event loop processing was ended
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
