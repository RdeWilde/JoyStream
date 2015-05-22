/** Adapted from https://github.com/richmoore/qt-examples/blob/master/httpserver/httpserver.h */

#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <QObject>

class QTcpServer;

class HttpServer : public QObject
{
    Q_OBJECT
public:
    HttpServer(quint16 port, QObject *parent=0);
    ~HttpServer();

protected slots:
    void handleConnection();

private:
    QTcpServer *server;
};

#endif // HTTP_SERVER_HPP
