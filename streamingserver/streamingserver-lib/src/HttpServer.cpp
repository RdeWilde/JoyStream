/** Adapted from https://github.com/richmoore/qt-examples/blob/master/httpserver/httpserver.cpp */

#include "streamingserver/HttpServer.hpp"
#include "streamingserver/HttpConnectionHandler.hpp"

#include <QTcpSocket>

HttpServer::HttpServer(quint16 port, QObject *parent)
    : QObject(parent)
    , _server(this) {

    // Connect server signals to http server object slot
    QObject::connect(&_server,
                     SIGNAL(newConnection()),
                     this,
                     SLOT(handleConnection()));

    QObject::connect(&_server,
                     SIGNAL(acceptError(QAbstractSocket::SocketError)),
                     this,
                     SLOT(handleAcceptError(QAbstractSocket::SocketError)));

    // Start listening
    bool success = _server.listen(QHostAddress::Any, port);

    if(success)
        qDebug() << "Start server listening on port:" << port;
    else
        qDebug() << "Could not start server listening on port:" << port;
}

HttpServer::~HttpServer() {
    qDebug() << "Destroying server";
}

void HttpServer::handleConnection() {

    qDebug() << "New connection arrived.";

    // Create handler for each pending connection
    // socket is owned by _server
    while(QTcpSocket * socket = _server.nextPendingConnection())
        new HttpConnectionHandler(socket, this);
}

void HttpServer::handleAcceptError(QAbstractSocket::SocketError socketError) {

    qDebug() << "Failed to accept connection.";
}
