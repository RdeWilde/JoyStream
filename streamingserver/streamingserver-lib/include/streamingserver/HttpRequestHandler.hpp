#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include <QObject>
#include <QMultiMap>

class QTcpSocket;

class HttpRequestHandler : public QObject
{
    Q_OBJECT

public:
    enum State {
        StateReadingRequest,
        StateReadingHeaders,
        StateDone
    };

    HttpRequestHandler(QTcpSocket *sock, QObject *parent=0);
    ~HttpRequestHandler();

    QTcpSocket *socket() const;
    QByteArray path() const;

protected:
    virtual void processRequest();

private slots:
    void dataReceived();

private:
    void sendError(int code);

private:
    State state;
    QTcpSocket *sock;
    QByteArray mpath;
    QMultiMap<QByteArray, QByteArray> headers;
};

#endif // HTTP_REQUEST_HANDLER_HPP
