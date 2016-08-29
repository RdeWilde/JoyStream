#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create the objects we need here to pass to the daemon server

    // Start a daemon on a new thread

    // We need a Qt event loop
    return a.exec();
}
