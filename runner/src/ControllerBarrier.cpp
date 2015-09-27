/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <runner/ControllerBarrier.hpp>
#include <core/controller/Controller.hpp>

namespace Runner {

ControllerBarrier::ControllerBarrier()
       : _controllerCount(0) {
}

void ControllerBarrier::add(Controller * controller) {

    // Connect closed signal to runner clientClosed slot
    QObject::connect(controller,
                     &Controller::closed,
                     &_loop,
                     &QEventLoop::quit);

    // Count client
    _controllerCount++;
}

void ControllerBarrier::erect() {

    // While there are still controllers not done
    while(_controllerCount > 0) {

        // (re)Start event loop
        _loop.exec();

        // We woke up, that means a controller stopped, so lets decrease count
        _controllerCount--;
    }

}

}
