/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef CONTROLLER_BARRIER_HPP
#define CONTROLLER_BARRIER_HPP

#include <QObject>
#include <QEventLoop>

class Controller;

namespace Runner {

// Barrier for running multiple controller, e.g. during testing
// Move back into controller lib????
// Move back into controller lib????
// Move back into controller lib????
class ControllerBarrier : public QObject
{
    Q_OBJECT

public:

    // Constructor
    ControllerBarrier();

    // Add client to run
    void add(Controller * controller);

    // Blocks untill controllerCount==0
    void join();

private:

    // Number of controllers
    int _controllerCount;

    // Underlying event loop which is executed ones per controllerCount registered
    QEventLoop _loop;

};

}

#endif // CONTROLLER_BARRIER_HPP
