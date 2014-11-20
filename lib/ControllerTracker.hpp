#ifndef CONTROLLER_TRACKER_HPP
#define CONTROLLER_TRACKER_HPP

#include <QObject>

#include "controller/Controller.hpp"

class ControllerTracker : public QObject
{
    Q_OBJECT

public:

    // Constructor
    ControllerTracker();

    // Add client to run
    void addClient(Controller * controller);

private:

    // Number of controllers
    int count;

public slots:

    // Accepts signal from controllers
    void controllerClosed();

};

#endif // CONTROLLER_TRACKER_HPP
