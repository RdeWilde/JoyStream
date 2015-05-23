#include "streamingserver/Stream.hpp"

#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>

static qint64 MAX_REQUEST_LINE_LENGTH = 2048;

Stream::Stream(QTcpSocket * socket, QObject * parent)
    : QObject(parent),
      _state(State::WaitingForFirstRequestLine),
      _socket(socket) {

    // Connect connection data arrival signal to handler slot
    QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
}

Stream::~Stream() {

    // Socket is owned by Streaming server, but lets delete it
    // when possible: Why not now?
    _socket->deleteLater();
}

void Stream::readSocket() {

    switch(_state) {

        case State::WaitingForFirstRequestLine:
            // Just do the same thing as State::ReadingRequestLine case

        case State::ReadingRequestLine:
            readRequestLine();
            break;

        case State::ReadingRequestHeaders:
            readRequestHeaders();
            break;

        case State::Done:

            if (_socket->bytesAvailable())
                qWarning("Data received when done!");

            break;

        case State::Error:
            qWarning("Data received in error state.");
            break;
    }

}

void Stream::sendDataRange(int start, const char * data) {

}

void Stream::readRequestLine() {

    // If we have a full line, then parse it
    if (_socket->canReadLine()) {

        Q_ASSERT(_state == State::WaitingForFirstRequestLine ||
                 _state == State::ReadingRequestLine);

        // Read request line
        QByteArray requestLine = _socket->readLine();

        // Split to recover method used
        QList<QByteArray> requestLineSections = requestLine.split(' ');

        // We only support GET
        if (requestLineSections[0].toUpper() != QByteArrayLiteral("GET")) {

            qDebug() << "Not a GET request line:" << requestLineSections[0];

            // End stream
            endOnError(Error::NotAGET);

        } else {

            // Save requested path if this is the
            if(_state == State::WaitingForFirstRequestLine) {

                qDebug() << "Got request for:" << requestLineSections[1];

                Q_ASSERT(_requestedPath.size() == 0);

                // Save requested path
                _requestedPath = requestLineSections[1];

            } else if(_requestedPath != requestLineSections[1]) {

                qDebug() << "Changed request path from" << _requestedPath << "to" << requestLineSections[1];

                // End stream
                endOnError(Error::ChangedRequestPath);

                // Done
                return;
            }

            // Update state
            _state = State::ReadingRequestHeaders;

            // Try to read headers in case they are readable
            readRequestHeaders();
        }
    }
    // If we dont have a full line, yet more than the given limit, then end stream
    else if (_socket->bytesAvailable() > MAX_REQUEST_LINE_LENGTH)
        endOnError(Error::RequestLineLengthToLong);
}

void Stream::readRequestHeaders() {

    Q_ASSERT(_state == State::ReadingRequestHeaders);

    // While there is a full line to read, read it
    while (_socket->canReadLine()) {

        QByteArray headerLine = _socket->readLine();

        // If all headers have now been read, then process full request
        if (headerLine == QByteArrayLiteral("\r\n") || headerLine == QByteArrayLiteral("\n"))
            processRequest();
        else {

            // Split header line to recover name and value
            int indexOfFirstColon = headerLine.indexOf(':');

            // Check that splitting is possible, i.e. there is at least one colon
            if (indexOfFirstColon == -1) {

                qDebug() << "Header line is missing colon:" << headerLine;

                endOnError(Error::HeaderLineMissingColon);

            } else {

                // Grab header name and value
                QByteArray header = headerLine.left(indexOfFirstColon);
                QByteArray value = headerLine.mid(indexOfFirstColon+1);

                // Insert into header map
                _headers.insert(header.toUpper(), value.trimmed());

                qDebug() << header.toUpper() << value.trimmed();
            }
        }
    }
}

void Stream::processRequest() {

    QString headers("HTTP/1.1 200 OK\n" \
                    "Content-Length: %1\n" \
                    "\n");

    QString body("<html><body>" \
                 "<h1>It works!</h1>"         \
                 "HTTP connection from %1"    \
                 "</body></html>");

    body = body.arg(_socket->peerAddress().toString());

    _socket->write(headers.arg(body.toUtf8().length()).toUtf8());
    _socket->write(body.toUtf8());
    _socket->flush(); // Needed before change 86186

    // Reset for next request
    _state = State::ReadingRequestLine;
    _requestedPath.clear();
    _headers.clear();
}

void Stream::sendError(int code) {

    QString headers("HTTP/1.1 %1 FAIL\n" \
                    "Content-Length: %2\n" \
                    "\n");

    QByteArray body("<html><body><h1>Error :-(</h1></body></html>");

    _socket->write(headers.arg(code).arg(body.length()).toUtf8());
    _socket->write(body);
    _socket->flush(); // Needed before change 86186
}

QByteArray Stream::requestedPath() const {
    return _requestedPath;
}

void Stream::endOnError(Error error) {

    // Update to error state
    _state = State::Error;

    // Notify about error condition
    emit errorOccured(error);

    // Send error to client
    sendError(400);

    // Close socket
    _socket->close();

    // Delete *this stream later from event loop
    deleteLater();
}
