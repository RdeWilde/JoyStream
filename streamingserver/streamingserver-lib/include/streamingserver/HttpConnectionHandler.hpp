/** Adapted from https://github.com/richmoore/qt-examples/blob/master/httpserver/httprequesthandler.h **/

#ifndef HTTP_CONNECTION_HANDLER_HPP
#define HTTP_CONNECTION_HANDLER_HPP

#include <QObject>
#include <QMultiMap>

class QTcpSocket;

class HttpConnectionHandler : public QObject
{
    Q_OBJECT

public:

    // Handler state
    enum class State {
        ReadingRequest,
        ReadingHeaders,
        Done,
        Error
    };

    // Constructor
    HttpConnectionHandler(QTcpSocket * sock, QObject * parent = 0);

protected:

    virtual void processRequest();

private slots:

    void dataReceived();

private:

    void readRequest();
    void readingHeaders();

    void sendError(int code);

private:

    // Sate of handler
    State _state;

    // Socket for connection
    QTcpSocket * _sock;

    // Contains path requested, is only valid
    // when _state != ReadingRequest
    QByteArray _mpath;

    //
    QMultiMap<QByteArray, QByteArray> _headers;
};

#endif // HTTP_CONNECTION_HANDLER_HPP
