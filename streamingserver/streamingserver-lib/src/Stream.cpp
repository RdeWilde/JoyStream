#include "streamingserver/Stream.hpp"

#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>

static qint64 MAX_REQUEST_LINE_LENGTH = 2048;

Stream::Stream(QTcpSocket * socket, QObject * parent)
    : QObject(parent)
    , _state(State::WaitingForFirstRequestLine)
    , _socket(socket) {

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

void Stream::sendDataRange(const QString & contentType,
                           const QList<const boost::shared_array<char> > & data,
                           int offsetInFirstChunk,
                           int offsetInLastArray) {

    qDebug() << "sendDataRange" << contentType << "," << offsetInFirstChunk << "," << offsetInLastArray;

    // LOOK AT RESPONSE IN BROWSER

    // Check that this is data for the most recent
    // request we issue :_mostRecentlyRequestedRange
    /**
            res.writeHead(206, {
                "Content-Range": "bytes " + start + "-" + end + "/" + total,
                "Accept-Ranges": "bytes",
                "Content-Length": chunksize,
                "Content-Type": "video/mp4"
            });


      - Either a Content-Range header field (section 14.16) indicating
        the range included with this response, or a multipart/byteranges
        Content-Type including Content-Range fields for each part. If a
        Content-Length header field is present in the response, its
        value MUST match the actual number of OCTETs transmitted in the
        message-body.
      - Date
      - ETag and/or Content-Location, if the header would have been sent
        in a 200 response to the same request
      - Expires, Cache-Control, and/or Vary, if the field-value might
        differ from that sent in any previous response for the same
        variant


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
    */

}

void Stream::invalidRangeRequested(int start) {

    qDebug() << "invalidRangeRequested" << start;

    // End stream
    endOnError(Error::InvalidRangeRequested);
}

void Stream::readRequestLine() {

    // If we have a full line, then parse it
    if (_socket->canReadLine()) {

        Q_ASSERT((_state == State::WaitingForFirstRequestLine && _requestedPath.size() == 0) ||
                 (_state == State::ReadingRequestLine && _requestedPath.size() != 0));

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
    Q_ASSERT(_requestedPath.size() != 0);

    // While there is a full line to read, read it
    while (_socket->canReadLine()) {

        // Read header line
        QByteArray headerLine = _socket->readLine();

        //qDebug() << "Read header line:" << headerLine;

        // If all headers have now been read, then process full request
        if (headerLine == QByteArrayLiteral("\r\n") || headerLine == QByteArrayLiteral("\n"))
            processRequest();
        else {

            // Split header line to recover name and value
            bool ok;
            QPair<QByteArray, QByteArray> splittedHeaderLine = splitInHalf(headerLine, ':', ok);

            // Was splitting possible?
            if(ok) {

                qDebug() << splittedHeaderLine.first << ':'<< splittedHeaderLine.second;

                // Insert into header map
                _headers.insert(splittedHeaderLine.first.toUpper(), splittedHeaderLine.second.trimmed());

            } else {

                qDebug() << "Header line is missing colon:" << headerLine;

                // End
                endOnError(Error::HeaderLineMissingColon);
            }
        }
    }
}

void Stream::processRequest() {

    Q_ASSERT(_state == State::ReadingRequestHeaders);
    Q_ASSERT(_requestedPath.size() != 0);

    // Try to recover range field
    QByteArray range = QByteArrayLiteral("RANGE");

    // If range exists, try to use it
    if(_headers.contains(range)) {

        // Lets just assume there is only one range field for now
        QList<QByteArray> headers = _headers.values("plenty");

        Q_ASSERT(headers.size() == 1);

        QByteArray rangeValue = headers.front();

        // Remove "bytes"
        rangeValue.replace("bytes","");

        // Split range into start and end, if possible.
        bool ok;
        QPair<QByteArray, QByteArray> splittedRangeValue = splitInHalf(rangeValue, '-', ok);

        // Was it possible?
        if(ok) {

            bool couldConvertStartOfRange;
            int start = splittedRangeValue.first.toInt(&couldConvertStartOfRange);

            bool couldConvertEndOfRange;
            int end = splittedRangeValue.first.toInt(&couldConvertEndOfRange);

            // Could actual range values be recovered?
            if(couldConvertStartOfRange && couldConvertEndOfRange) {

                // Make request for spesific range
                qDebug() << "Requesting range [" << start << "," << end << "]";

                // Save range start
                _mostRecentlyRequestedStartOfRange = start;

                // Send range request notification
                emit rangeRequested(start, end);

            } else {

                qDebug() << "Could not parse range header line:" << rangeValue;

                // End
                endOnError(Error::InvalidRangeHeaderLine);

                return;
            }

        } else {

            // Try to see if we can just recover start of range
            bool couldConvertStartOfRange;
            int start = rangeValue.toInt(&couldConvertStartOfRange);

            if(couldConvertStartOfRange)  {

                // Make request for start
                qDebug() << "Requesting start" << start;

                // Save range start
                _mostRecentlyRequestedStartOfRange = start;

                // Send range request notification
                emit startRequested(start);

            } else {

                qDebug() << "Could not parse range header line:" << rangeValue;

                // End
                endOnError(Error::InvalidRangeHeaderLine);

                return;
            }

        }

    } else {

        // Just dump first part of file?
        qDebug() << "Requesting 0";

        // Save range start
        _mostRecentlyRequestedStartOfRange = 0;

        // Send range request notification
        emit startRequested(0);
    }

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

QPair<QByteArray, QByteArray> Stream::splitInHalf(QByteArray data, char c, bool & ok) {

    // Return value
    QPair<QByteArray, QByteArray> returnValue;

    // Split line
    int indexOfFirstColon = data.indexOf(c);

    // Check that splitting is possible, i.e. there is at least one delimiter
    if (indexOfFirstColon == -1)
        ok = false;
    else {
        ok = true;
        returnValue.first = data.left(indexOfFirstColon);
        returnValue.second = data.mid(indexOfFirstColon+1);
    }

    return returnValue;
}
