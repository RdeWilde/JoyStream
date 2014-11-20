#include "BitSwapr.hpp"
#include "controller/Controller.hpp"
#include "controller/Exceptions/ListenOnException.hpp"

BitSwapr::BitSwapr(const ControllerState & controllerState, bool showView, QLoggingCategory * category)
    : controllerState_(controllerState)
    , view_(showView ? new MainWindow() : 0)
    , controller_(0)
    , category_(category == 0 ? QLoggingCategory::defaultCategory() : category) {

    if(showView)
        view_->show();

    /*
    // Have runner call runner_entry() when it starts
    QObject::connect(&runner, SIGNAL(started()), this, SLOT(runner_entry()));

    // Have runner call runner_exit() routine when it stops processing event loop
    QObject::connect(&runner, SIGNAL(finished()), this, SLOT(runner_exit()));
    */
}

BitSwapr::~BitSwapr() {
    qCDebug(CATEGORY) << "~BitSwapr() called.";

    // Delete view if we used one
    // NB!: This deletion must be done by same thread which
    // called constructor, and therefor created view_.
    if(view_ != 0)
        delete view_;
}

/*
void BitSwapr::start() {

    // Start thread
    // Calls run() on new runner thread, and run() calls exec() by default,
    // which is event loop entry point.
    runner.start();
}
*/

void BitSwapr::stop() {

    // Tell controller to stop, must be done on owning thread
    QMetaObject::invokeMethod(controller_, "begin_close");
}

// All objects created here are owned by this object
void BitSwapr::run() {

    // Do not allow client to run twice,
    // since old controller is not deleted.
    // IMPLEMENT AS ASSERT?
    if(controller_ != 0) {
        qCCritical(CATEGORY) << "BitSwapr start() routine called more than once, returning.";
        return;
    }

    std::cout << "Thread id of runner_entry() runner = " << QThread::currentThreadId() << std::endl;

    // Create controller
    try {
        controller_ = new Controller(controllerState_, category_);
    } catch(ListenOnException & e) {
        qCCritical(CATEGORY) << "ERROR: failed to start libtorrent listening due to " <<  e.what() << ".";
        exit(EXIT_FAILURE);
    }

    std::cout << "Created controller." << std::endl;

    // Connect with view
    if(view_ != 0)
        controller_->connectToView(view_);

    // Start servicing event loop for controller
    // finalize_close in controller causes us to return from loop
    exec();

    // Delete controller, should be done by same thread, e.g. so timer
    // in controller does not complain
    delete controller_;
}

/*
void BitSwapr::runner_exit() {
    qCDebug(CATEGORY) << "Client was stopped.";
}
*/
