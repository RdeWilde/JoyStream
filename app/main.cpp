#include <QApplication>
#include <QString>
#include <QDir>

#include <iostream>

// LATER REPLACE BOOST COMMAND LINE PROCESSING WITH QCommandLineParser
#ifndef Q_MOC_RUN
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#endif Q_MOC_RUN

#include "controller/ControllerState.hpp"
#include "BitSwapr.hpp"
#include "Config.hpp"

// Forward declarations
bool updateManager();

void main(int argc, char* argv[]) {

    // Create Qt application: all objects created after this point are owned by this thread
    QApplication a(argc, argv);

    // Create options description
    po::options_description desc("Allowed options");

    // Add options
    desc.add_options()
      ("help,h", "Print help messages")
      ("no-update,n", "Do not run update manager")
      ("console,c", "Run in console mode")
      ("fresh,f", "Create and use a fresh parameter file");

    // Parse the command line catching and displaying any parser errors
    po::variables_map vm;

    try {

        // Parse
        po::store(po::parse_command_line(argc, argv, desc), vm);

        // Throw exception if required option is not present
        po::notify(vm);

    } catch(po::error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl;
        std::cerr << desc << std::endl;
        exit(EXIT_FAILURE);
    }

    // Display help text when requested
    if(vm.count("help")) {
        std::cout << desc << std::endl;
        return;
    }

    // Call update manager, if allowed
    if(!vm.count("no-update")) {

        bool doRestart = updateManager();

        if(doRestart) {
            // Call update runner
        }
    }

    // Check console flag
    bool showView = false;
    if(!vm.count("console"))
        showView = true;

    // Load default state
    ControllerState controllerState;

    // If fresh flag is not passed,
    // then open existing parameter file
    if(!vm.count("fresh")) {

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

    // Create client
    BitSwapr client(controllerState, showView);

    // Start client
    client.start();
    std::cout << "Started client thread." << std::endl;

    // Start application event loop,
    // it services events of thread which runs client.
    a.exec();

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
