#ifndef STREAMING_SERVER_HPP
#define STREAMING_SERVER_HPP

#include <QObject>
#include <QTcpServer>
#include <QAbstractSocket> // is nested enum QAbstractSocket::SocketError

class Stream;

class StreamingServer : public QObject
{
    Q_OBJECT
public:

    // Constructor
    StreamingServer(quint16 port, QObject * parent = 0);

    // Destructor
    ~StreamingServer();

public slots:

    // Checks server for pending connections
    void handleConnection();

    // Checks server for pending error
    void handleAcceptError(QAbstractSocket::SocketError socketError);

signals:

    // A stream was started which is handled by the given handler
    void streamStarted(const Stream * handler);

    // A stream could not be created due to given socket error
    void streamCreationError(QAbstractSocket::SocketError socketError);

private:


};

#endif // STREAMING_SERVER_HPP
