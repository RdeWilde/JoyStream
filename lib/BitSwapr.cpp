#include "BitSwapr.hpp"
#include "controller/Exceptions/ListenOnException.hpp"

BitSwapr::BitSwapr(const ControllerState & controllerState, bool showView, QLoggingCategory * category)
    : controllerState_(controllerState)
    , controller_(0)
    , showView_(showView)
    , category_(category == 0 ? QLoggingCategory::defaultCategory() : category) {

    // Have runner call runner_entry() when it starts
    QObject::connect(&runner, SIGNAL(started()), this, SLOT(runner_entry()));

    // Have runner call runner_exit() routine when it stops processing event loop
    QObject::connect(&runner, SIGNAL(finished()), this, SLOT(runner_exit()));
}

BitSwapr::~BitSwapr() {

    qCDebug(CATEGORY) << "~BitSwapr() called.";

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

    std::cout << "Thread id of runner_entry() runner = " << QThread::currentThreadId() << std::endl;

    try {
        // Create controller
        controller_ = new Controller(controllerState_, showView_, category_);
    } catch(ListenOnException & e) {
        qCCritical(CATEGORY) << "ERROR: failed to start libtorrent listening due to " <<  e.what() << ".";
        exit(EXIT_FAILURE);
    }

    // Start servicing event loop
}

void BitSwapr::runner_exit() {
    qCDebug(CATEGORY) << "Client was stopped.";
}
