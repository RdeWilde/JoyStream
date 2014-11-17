#include "BitSwapr.hpp"
#include "controller/Exceptions/ListenOnException.hpp"

BitSwapr::BitSwapr(const ControllerState & controllerState, bool showView)
    : controllerState_(controllerState)
    , controller_(0)
    , showView_(showView) {

    // Have runner call runner_entry() when it starts
    QObject::connect(&runner, SIGNAL(started()), this, SLOT(runner_entry()));

    // Have runner call runner_exit() routine when it stops processing event loop
    QObject::connect(&runner, SIGNAL(finished()), this, SLOT(runner_exit()));
}

BitSwapr::~BitSwapr() {

    std::cout << "~BitSwapr() called." << std::endl;

    // No need to delete controller_ here, it deletes itself
    // and then tells runner to stop event loop.
}

void BitSwapr::start() {

    // Start thread
    // Calls run() on new runner thread, and run() calls exec() by default,
    // which is event loop entry point.
    runner.start();
}

void BitSwapr::stop() {

    // Tell runner to close controller, cannot be done on another thread
    QMetaObject::invokeMethod(controller_, "begin_close");
}

void BitSwapr::runner_entry() {

    // All objects created from here on in are owned by runner

    try {
        // Create controller
        controller_ = new Controller(controllerState_, showView_);
    } catch(ListenOnException & e) {
        std::cerr << "ERROR: failed to start libtorrent listening due to " <<  e.what() << "." << std::endl << std::endl;
        exit(EXIT_FAILURE);
    }
}

void BitSwapr::runner_exit() {
    std::cout << "Client was stopped." << std::endl;
}
