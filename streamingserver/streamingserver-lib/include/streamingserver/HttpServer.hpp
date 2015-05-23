/** Adapted from https://github.com/richmoore/qt-examples/blob/master/httpserver/httpserver.h */

#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <QObject>
#include <QTcpServer>
#include <QAbstractSocket> // is nested enum QAbstractSocket::SocketError

class HttpServer : public QObject
{
    Q_OBJECT
public:

    // Constructor
    HttpServer(quint16 port, QObject * parent = 0);

    ~HttpServer();

public slots:

    // Checks server for pending connections
    void handleConnection();

    // Checks server for pending error
    void handleAcceptError(QAbstractSocket::SocketError socketError);

signals:

    //void streamStarted

private:

    // TCP server
    QTcpServer _server;
};

#endif // HTTP_SERVER_HPP
