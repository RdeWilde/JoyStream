#include <QCoreApplication>

#include <streamingserver/StreamingServer.hpp>

int main(int argc, char *argv[]) {

    QCoreApplication app(argc, argv);
    StreamingServer server(9887, &app);

    qDebug() << "About to enter event loop";

    app.exec();

    qDebug() << "About to exit event loop";

}
