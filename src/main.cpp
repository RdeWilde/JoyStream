#include <QApplication>
#include <QThread>

#include <iostream>

// LATER REPLACE BOOST COMMAND LINE PROCESSING WITH QCommandLineParser
#ifndef Q_MOC_RUN

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

// Name space abbreviations
namespace po = boost::program_options;
namespace fs = boost::filesystem;

#endif Q_MOC_RUN

#include "controller/include/Controller.hpp"
#include "controller/include/ControllerState.hpp"
#include "controller/include/Exceptions/ListenOnException.hpp"
#include "controller/include/Exceptions/MissingInfoHashViewRequestException.hpp"

// Forward declarations
bool updateManager();

void main(int argc, char* argv[]) {

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

    // Create Qt application: all objects created after this point are owned by this thread
    QApplication a(argc, argv);

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

    // Create controller
    Controller * controller;

    try {
        controller = new Controller(*state);
    } catch(ListenOnException & e) {
        std::cerr << "ERROR: failed to start libtorrent listening due to " <<  e.what() << "." << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }

    // Start application event loop
    a.exec();
}

bool updateManager() {

    // If we have just downloaded a new binary, then run it and then quit

    // Query blockchain or most recent location

    // Connect to location and check what version is there

    // If its newer, download it to particular location

    // then return to restart:

    return true;
}
