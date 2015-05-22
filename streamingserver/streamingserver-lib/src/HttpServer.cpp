/** Adapted from https://github.com/richmoore/qt-examples/blob/master/httpserver/httpserver.cpp */

#include "streamingserver/HttpServer.hpp"
#include "streamingserver/HttpRequestHandler.hpp"

#include <QTcpServer>
#include <QTcpSocket>

HttpServer::HttpServer(quint16 port, QObject *parent)
    : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(handleConnection()));
    server->listen(QHostAddress::Any, port);
}

HttpServer::~HttpServer() {
    // Memory leak
}

void HttpServer::handleConnection()
{
    while(server->hasPendingConnections()) {
        QTcpSocket *sock = server->nextPendingConnection();
        HttpRequestHandler *handler = new HttpRequestHandler(sock, this);
    }
}
