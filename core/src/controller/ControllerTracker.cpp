/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/controller/ControllerTracker.hpp>
#include <QThread>

ControllerTracker::ControllerTracker()
       : _controllerCount(0) {

}

void ControllerTracker::addClient(Controller * controller) {

    // Connect closed signal to runner clientClosed slot
    QObject::connect(controller, SIGNAL(closed()), this, SLOT(controllerClosed()));

    // Count client
    _controllerCount++;
}

void ControllerTracker::blockUntilAllControllersDone() {

    // While there are still controllers not done
    while(_controllerCount > 0) {

        // Start event loop
        _loop.exec();
    }

}

void ControllerTracker::controllerClosed() {

    // Decrase count
    _controllerCount--;

    // If all controllers are done, we kill event loop
    if(_controllerCount < 1) {
        QThread::currentThread()->exit();
    }
}
