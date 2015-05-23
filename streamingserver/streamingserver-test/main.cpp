#include <QCoreApplication>

#include <streamingserver/HttpServer.hpp>

int main(int argc, char *argv[]) {

    QCoreApplication app(argc, argv);
    HttpServer server(9887, &app);

    qDebug() << "About to enter event loop";

    app.exec();

    qDebug() << "About to exit event loop";

}
