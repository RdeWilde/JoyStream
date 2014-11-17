#ifndef BITSWAPR_HPP
#define BITSWAPR_HPP

#include "controller/ControllerState.hpp"
#include "controller/Controller.hpp"
#include <QThread>

class BitSwapr : public QObject
{
    Q_OBJECT

private:

    // State used when creating controller
    ControllerState controllerState_;

    // Controller
    Controller * controller_;

    // Whether to run with gui
    bool showView_;

    // Runs event loop for client
    QThread runner;

public:

    // Constructor
    BitSwapr(const ControllerState & controllerState, bool showView);

    // Destructor
    ~BitSwapr();

    // Starts BitSwapr
    void start();

    // Stop BitSwapr
    void stop();

public slots:

    // Called by runner thread when it starts
    void runner_entry();

    // Called by runner thread when it is stopped
    void runner_exit();
};

#endif // BITSWAPR_HPP
