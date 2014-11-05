#include <QApplication>

#include <iostream>

#ifndef Q_MOC_RUN
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#endif Q_MOC_RUN

#include "controller/include/Controller.hpp"
#include "controller/include/ControllerState.hpp"
#include "controller/include/Exceptions/ListenOnException.hpp"
#include "view/include/mainwindow.h"

// Forward declarations
void loadJVM();
bool updateManager();

// Name space abbreviations
namespace po = boost::program_options;
namespace fs = boost::filesystem;

void main(int argc, char* argv[]) {

    // Load JVM
    loadJVM();

    // Create options description
    po::options_description desc("Allowed options");

    // Add options
    desc.add_options()
      ("help,h", "Print help messages")
      ("console,c", "Run in console mode")
      ("fresh,f", "Create and use a fresh parameter file")
      ("no-update,n", "Do not run update manager");

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
    if(vm.count("help"))
        std::cout << desc << std::endl;

    // Call update manager, if allowed
    if(!vm.count("no-update")) {

        bool doRestart = updateManager();

        if(doRestart) {
            // Call update runner
        }
    }

    // Check console flag
    bool useConsole = false;
    if(!vm.count("console"))
        useConsole = true;

    // Open existing parameter file, or create a fresh one if user passed fresh flag
    ControllerState * state = NULL;
    if(!vm.count("fresh")) {

        // Get name of file name
        fs::path file = operator/(fs::current_path(), fs::path("parameter-file.txt")); // Parameters.DEFAULT_FILE_NAME

        // Check that file exists, and that it actually is a file
        if(!fs::exists(file)) {
            std::cerr << "ERROR: parameter file " << file << " does not exist." << std::endl << "Try fresh run option if problem persists" << std::endl << std::endl;
            exit(EXIT_FAILURE);
        } else if(!fs::is_regular_file(file)) {
            std::cerr << "ERROR: parameter file " << file << " is not regular." << std::endl << "Try fresh run option if problem persists" << std::endl << std::endl;
            exit(EXIT_FAILURE);
        }

        // Load state from file
        state = new ControllerState(file.string().c_str());
    } else // Load default state
        state = new ControllerState();

    // Create application
    QApplication a(argc, argv);

    // Create controller
    Controller * controller;

    try {
        controller = new Controller(*state);
    } catch(ListenOnException & e) {
        std::cerr << "ERROR: failed to start libtorrent listening due to " <<  e.what() << "." << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start session loop thread and show view
    controller->begin();

    // Start running Qt application event loop
    a.exec();
}

void loadJVM() {

}

bool updateManager() {

    // If we have just downloaded a new binary, then run it and then quit

    // Query blockchain or most recent location

    // Connect to location and check what version is there

    // If its newer, download it to particular location

    // then return to restart:

    return true;
}
