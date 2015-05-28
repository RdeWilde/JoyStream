/** Adapted from https://github.com/richmoore/qt-examples/blob/master/httpserver/httpserver.cpp */

#include "StreamingServer.hpp"
#include "Stream.hpp"

#include <QTcpSocket>

StreamingServer::StreamingServer(quint16 port, QObject *parent)
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

/**
void StreamingServer::announceRequest(const Stream * stream, const QByteArray & requestedPath) const{

}
*/

StreamingServer::~StreamingServer() {
    qDebug() << "Destroying server";
}

void StreamingServer::handleConnection() {

    qDebug() << "New connection arrived.";

    // Create handler for each pending connection
    // socket is owned by _server
    while(QTcpSocket * socket = _server.nextPendingConnection()) {

        // Create stream
        Stream * stream = new Stream(socket, this);

        // Notify about stream being started
        emit streamStarted(stream);
    }
}

void StreamingServer::handleAcceptError(QAbstractSocket::SocketError socketError) {

    qDebug() << "Failed to accept connection.";

    emit streamCreationError(socketError);
}
