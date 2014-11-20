#ifndef BITSWAPR_HPP
#define BITSWAPR_HPP

#include "controller/ControllerState.hpp"
#include "controller/Controller.hpp"
#include "view/MainWindow.hpp"

#include <QThread>
#include <QLoggingCategory>

// Used directing logging to category object.
#define CATEGORY (*category_)

// Yes, I am subclassing QThread, and it works great! jesus Qt community
class BitSwapr : public QThread
{
    Q_OBJECT

private:

    // State used when creating controller
    ControllerState controllerState_;

    // View
    MainWindow * view_;

    // Controller
    // We need pointer here so that an external thread can call
    // stop() to stop client.
    Controller * controller_;

    // Whether to run with gui
    //bool showView_;

    // Runs event loop for client
    //QThread runner;

    // Logging category
    QLoggingCategory * category_;

public:

    // Constructor
    BitSwapr(const ControllerState & controllerState, bool showView = true, QLoggingCategory * category = 0);

    // Destructor
    ~BitSwapr();

    // Starts BitSwapr
    //void start();

    // Thread entry point
    virtual void run();

    // Stop BitSwapr
    void stop();

    /*
public slots:

    // Called by runner thread when it starts
    void runner_entry();

    // Called by runner thread when it is stopped
    void runner_exit();
    */
};

#endif // BITSWAPR_HPP
