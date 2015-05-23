/** Adapted from https://github.com/richmoore/qt-examples/blob/master/httpserver/httprequesthandler.cpp **/

#include "streamingserver/HttpConnectionHandler.hpp"

#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>

static qint64 MAX_REQUEST_LINE_LENGTH = 2048;

HttpConnectionHandler::HttpConnectionHandler(QTcpSocket * sock, QObject * parent)
    : QObject(parent),
      _state(State::ReadingRequest),
      _sock(sock) {

    // Connect connection data arrival signal to handler slot
    QObject::connect(sock, SIGNAL(readyRead()), this, SLOT(dataReceived()));
}

void HttpConnectionHandler::dataReceived() {

    switch(_state) {

        case State::ReadingRequest:
            readRequest();
            break;

        case State::ReadingHeaders:
            readingHeaders();
            break;

        case State::Done:

            if (_sock->bytesAvailable())
                qWarning("Data received when done!");

            break;


        case State::Error:
            qWarning("Data received in error state.");
            break;
    }

}

void HttpConnectionHandler::readRequest() {

    if (_sock->canReadLine()) {

        QByteArray request = _sock->readLine();

        QList<QByteArray> requestSections = request.split(' ');

        // We only support GET
        if (requestSections[0].toUpper() != QByteArrayLiteral("GET")) {

            _state = State::Error;

            qDebug() << "Not a get";
            sendError(400);
            deleteLater();

            return;
        }

        _mpath = requestSections[1];
        _state = State::ReadingHeaders;

        // Fall through to reading headers
        qDebug() << "Got request for:" << _mpath;

        // Try to read headers in case they are readable
        readingHeaders();
    }
    else if (_sock->bytesAvailable() > MAX_REQUEST_LINE_LENGTH) {

        _state = State::Error;

        qDebug() << "Too long";
        sendError(400);
        deleteLater();

        return;
    }

}

void HttpConnectionHandler::readingHeaders() {

    while (_sock->canReadLine()) {

        QByteArray request = _sock->readLine();

        // Is this the end?
        if (request == QByteArrayLiteral("\r\n") || request == QByteArrayLiteral("\n")) {

            processRequest();
            _state = State::Done;
            return;
        }

        int colon = request.indexOf(':');

        if (colon == -1) {

            _state = State::Error;

            qDebug() << "Bad header" << request.toHex();
            sendError(400);
            deleteLater();
            return;
        }

        QByteArray header = request.left(colon);
        QByteArray value = request.mid(colon+1);

        qDebug() << header.toUpper() << value.trimmed();
        _headers.insert(header.toUpper(), value.trimmed());
    }
}

void HttpConnectionHandler::processRequest()
{
    QString headers("HTTP/1.1 200 OK\n" \
                    "Content-Length: %1\n" \
                    "\n");

    QString body("<html><body>" \
                 "<h1>It works!</h1>"         \
                 "HTTP connection from %1"    \
                 "</body></html>");
    body = body.arg(_sock->peerAddress().toString());

    _sock->write(headers.arg(body.toUtf8().length()).toUtf8());
    _sock->write(body.toUtf8());
    _sock->flush(); // Needed before change 86186
    _sock->close();
    _sock->deleteLater();

    // RESET!
}

void HttpConnectionHandler::sendError(int code)
{
    QString headers("HTTP/1.1 %1 FAIL\n" \
                    "Content-Length: %2\n" \
                    "\n");

    QByteArray body("<html><body><h1>Error :-(</h1></body></html>");

    _sock->write(headers.arg(code).arg(body.length()).toUtf8());
    _sock->write(body);
    _sock->flush(); // Needed before change 86186
    _sock->close();
    _sock->deleteLater();
}
