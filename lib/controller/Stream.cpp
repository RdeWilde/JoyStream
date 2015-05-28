#include "Stream.hpp"

/**
 * Stream::PieceRequest
 */

Stream::PieceRequest::PieceRequest(int index, int length, Status status, const boost::shared_array<char> & buffer)
    : _index(index)
    , _length(length)
    , _status(status)
    , _buffer(buffer) {
}

boost::shared_array<char> Stream::PieceRequest::buffer() const {
    return _buffer;
}

void Stream::PieceRequest::setBuffer(const boost::shared_array<char> & buffer) {
    _buffer = buffer;
}

Stream::PieceRequest::Status Stream::PieceRequest::status() const {
    return _status;
}

void Stream::PieceRequest::setStatus(Status status) {
    _status = status;
}

int Stream::PieceRequest::length() const {
    return _length;
}

void Stream::PieceRequest::setLength(int length) {
    _length = length;
}

int Stream::PieceRequest::index() const {
    return _index;
}

void Stream::PieceRequest::setIndex(int index) {
    _index = index;
}

/**
 * Stream
 */

#include "Controller.hpp"
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>

static qint64 MAX_REQUEST_LINE_LENGTH = 2048;

Stream::Stream(QTcpSocket * socket, Controller * controller)
    : QObject(controller)
    , _controller(controller)
    , _socketProcessingState(SocketProcessingState::WaitingForFirstRequestLine)
    , _socket(socket) {

    // Connect socket signals to stream slots
    QObject::connect(_socket,
                     SIGNAL(readyRead()),
                     this,
                     SLOT(readSocket()));

    QObject::connect(_socket,
                     SIGNAL(disconnected()),
                     this,
                     SLOT(socketWasClosed()));
}

Stream::~Stream() {

    // Socket is owned by original tcp server,
    // but lets delete it when possible: Why not now?
    _socket->deleteLater();
}

void Stream::pieceRead(const boost::shared_array<char> & buffer, int pieceIndex, int size) {

    // Do nothing if no range request is being serviced
    if(!_currentlyServicingRangeRequest)
        return;

    Q_ASSERT(!_pieceRequests.empty());

    // Skip pieces which are not relevant to present request range
    if(pieceIndex < _pieceRequests.front().index() || pieceIndex > _pieceRequests.last().index())
        return;

    // Get piece request
    PieceRequest & pieceRequest = _pieceRequests[pieceIndex - _pieceRequests.front().index()];

    Q_ASSERT(pieceRequest.length() == size);

    // Get status of piece request
    PieceRequest::Status pieceRequestStatus = pieceRequest.status();

    if(pieceRequestStatus == PieceRequest::Status::ready_in_buffer)
        return;
    else if(pieceRequestStatus == PieceRequest::Status::ready_in_buffer)
        qDebug() << "A piece which was failed to be read properly was no read properly?";
    else if(pieceRequestStatus == PieceRequest::Status::waiting_for_it_be_downloaded)
        qDebug() << "A piece which was waiting to be downloaded was all of a sudden read."; // This can happen if some one else  beats us to the punch to ask for a read

    // Update state
    pieceRequest.setStatus(PieceRequest::Status::ready_in_buffer);

    // Save data
    pieceRequest.setBuffer(buffer);

    // Count piece as read
    _numberOfPiecesNotRead--;

    // Do we have all pieces now?
    if(_numberOfPiecesNotRead == 0) {

        // Send response to client
        sendStream();

        // Clear out requests
        _pieceRequests.clear();
    }
}

void Stream::pieceFinished(int pieceIndex) {

    // Do nothing if no range request is being serviced
    if(!_currentlyServicingRangeRequest)
        return;

    Q_ASSERT(!_pieceRequests.empty());

    // Skip pieces which are not relevant to present request range
    if(pieceIndex < _pieceRequests.front().index() || pieceIndex > _pieceRequests.last().index())
        return;

    // Get piece request
    PieceRequest pieceRequest = _pieceRequests[pieceIndex - _pieceRequests.front().index()];

    if(pieceRequest.status() == PieceRequest::Status::waiting_for_it_be_downloaded) {

        // Update state
        pieceRequest.setStatus(PieceRequest::Status::waiting_for_it_to_be_read);

        // Request piece be read
        _handle.read_piece(pieceIndex);
    } else {

        // It is impossible that we were waiting for a piece to be read which was not finished
        // is this true: put some solid invariant here later
        //Q_ASSER(pieceRequest.status() == PieceRequest::Status::waiting_for_it_to_be_read);
    }
}

void Stream::readSocket() {

    switch(_socketProcessingState) {

        case SocketProcessingState::WaitingForFirstRequestLine:
            // Just do the same thing as State::ReadingRequestLine case

        case SocketProcessingState::ReadingRequestLine:
            readRequestLineFromSocket();
            break;

        case SocketProcessingState::ReadingRequestHeaders:
            readRequestHeadersFromSocket();
            break;

        /**
        case SocketProcessingState::Done:

            if (_socket->bytesAvailable())
                qWarning("Data received when done!");

            break;
        */

        case SocketProcessingState::SocketClosedByPeer:
            qWarning("Data received after socket closed by peer?");
            Q_ASSERT(false);
            break;

        case SocketProcessingState::SocketClosedByUs:
            qWarning("Read socket after socket was closed by us? we may be in error state also.");
            Q_ASSERT(false);
            break;
    }

}

void Stream::socketWasClosed() {

    qDebug() << "Socket closed.";

    // This signal cannot arrive if peer has already closed socket,
    // as this can only happen by running this slot before.
    Q_ASSERT(_socketProcessingState != SocketProcessingState::SocketClosedByPeer);

    // If socket has already been closed by us, either gracefully or since
    // there was some sort of error, then we dont need to do anything
    if(_socketProcessingState == SocketProcessingState::SocketClosedByUs)
        return;

    /**
     * if we are here, then this socket was closed by peer, which we have to handle.
     */
    _socketProcessingState = SocketProcessingState::SocketClosedByPeer;

    // Notify controller
    _controller->unRegisterStream(this);

    // Delete *this stream later from event loop
    deleteLater();
}

void Stream::readRequestLineFromSocket() {

    // If we have a full line, then parse it
    if (_socket->canReadLine()) {

        // Make sure we are supposed to be reading request line
        Q_ASSERT((_socketProcessingState == SocketProcessingState::WaitingForFirstRequestLine && _requestedPath.size() == 0) ||
                 (_socketProcessingState == SocketProcessingState::ReadingRequestLine && _requestedPath.size() != 0));

        // Read request line
        QByteArray requestLine = _socket->readLine();

        // Split to recover method used
        QList<QByteArray> requestLineSections = requestLine.split(' ');

        // We only support GET
        if (requestLineSections[0].toUpper() != QByteArrayLiteral("GET")) {

            qDebug() << "Not a GET request line:" << requestLineSections[0];

            // End stream
            sendErrorToPeerAndEndStream(Error::NotAGET);

        } else {

            // Save requested path if this is the first time we see it
            if(_socketProcessingState == SocketProcessingState::WaitingForFirstRequestLine) {

                qDebug() << "Got request for:" << requestLineSections[1];

                Q_ASSERT(_requestedPath.size() == 0);

                // Save requested path
                _requestedPath = requestLineSections[1];

                // Try to get torrent handle and subscribe to piece events if this request is valid
                _handle = _controller->registerStream(this);

                // End if it was not valid
                if(!handle.is_valid()) {

                    qDebug() << "Not a valid info_hash:" << _requestedPath;
                    sendErrorToPeerAndEndStream(Error::InvalidInfoHash);
                    return;
                }

                /**
                 * Setup information about torrent
                 * IN THE FUTURE THIS NEEDS TO BE MORE CLEVER AND MAY INVOLVE USER INPUT,
                 * THERE ARE A FEW GITHUB ISSUES ON THIS.
                 */

                boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfo =_handle.torrent_file();

                // We should always have metadata if a stream has been started
                Q_ASSERT(torrentInfo != NULL);

                _defaultRangeLength = torrentInfo->piece_length() * 10;
                _fileIndex = 0;
                _total = torrentInfo->file_at(_fileIndex).size;
                _contentType = "video/mp4";
                _defaultRangeLength = 1024*1024; // 1 MB
                _currentlyServicingRangeRequest = false;

            } // If its not the first time, make sure its the same path, otherwise the peer is misbehaving
            else if(_requestedPath != requestLineSections[1]) {

                qDebug() << "Changed request path from" << _requestedPath << "to" << requestLineSections[1];

                // End stream
                sendErrorToPeerAndEndStream(Error::ChangedRequestPath);

                // Done
                return;
            }

            // Update state
            _socketProcessingState = SocketProcessingState::ReadingRequestHeaders;

            // Try to read headers in case they are readable
            readRequestHeadersFromSocket();
        }
    }
    // If we dont have a full line, yet more than the given limit, then end stream
    else if (_socket->bytesAvailable() > MAX_REQUEST_LINE_LENGTH)
        sendErrorToPeerAndEndStream(Error::RequestLineLengthToLong);

    // In case we ended header processing due to end of header, lets
    // try to process the next request in case it has arrived
    readRequestLineFromSocket();
}

void Stream::readRequestHeadersFromSocket() {

    Q_ASSERT(_socketProcessingState == SocketProcessingState::ReadingRequestHeaders);
    Q_ASSERT(_requestedPath.size() != 0);

    // While there is a full line to read, read it
    while (_socket->canReadLine()) {

        // Read header line
        QByteArray headerLine = _socket->readLine();

        //qDebug() << "Read header line:" << headerLine;

        // If all headers have now been read, then process full request
        if (headerLine == QByteArrayLiteral("\r\n") || headerLine == QByteArrayLiteral("\n")) {

            // Process request
            processRequest();

            // end header reading
            break;

        } else {

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
                sendErrorToPeerAndEndStream(Error::HeaderLineMissingColon);
            }
        }
    }
}


QPair<QByteArray, QByteArray> Stream::splitInHalf(QByteArray data, char c, bool & ok) const {

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

void Stream::sendErrorToPeerAndEndStream(Error error) {

    // Update to error state
    _socketProcessingState = SocketProcessingState::SocketClosedByUs;

    // Unregister from controller
    _controller->unRegisterStream(this, error);

    // Send 400 error to client
    QString headers("HTTP/1.1 %1 FAIL\n" \
                    "Content-Length: %2\n" \
                    "\n");

    QByteArray body("<html><body><h1>Error :-(</h1></body></html>");

    _socket->write(headers.arg(400).arg(body.length()).toUtf8());
    _socket->write(body);
    _socket->flush(); // Needed before change 86186

    // Close socket
    _socket->close();

    // Delete *this stream later from event loop
    deleteLater();
}


void Stream::processRequest() {

    Q_ASSERT(_socketProcessingState == SocketProcessingState::ReadingRequestHeaders);
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
                getStreamPieces(start, end);

            } else {

                qDebug() << "Could not parse range header line:" << rangeValue;

                // End
                sendErrorToPeerAndEndStream(Error::InvalidRangeHeaderLine);

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
                getStreamPieces(start);

            } else {

                qDebug() << "Could not parse range header line:" << rangeValue;

                // End
                sendErrorToPeerAndEndStream(Error::InvalidRangeHeaderLine);

                return;
            }

        }

    } else {

        // Just dump first part of file?
        qDebug() << "Requesting 0";

        // Save range start
        _mostRecentlyRequestedStartOfRange = 0;

        // Send range request notification
        getStreamPieces(0);
    }

    // Reset for next request
    _socketProcessingState = SocketProcessingState::ReadingRequestLine;
    _requestedPath.clear();
    _headers.clear();
}

void Stream::getStreamPieces(int start, int end) {

    // Check that the requested range is valid
    if(start < 0 || end >= _total || end <= start) {

        qDebug() << "Invalid range request [" << start << "," << end << "]";

        // End it
        sendErrorToPeerAndEndStream(Error::InvalidRangeRequested);

        return;
    }

    if(_currentlyServicingRangeRequest)
        qDebug() << "Dropping currently active request for [" << _start << "," << _end << "]";

    /**
     * Note that any previously active request will simply
     * be overwritten below
     */

    // The only way there are pending requests is if
    Q_ASSERT(_pieceRequests.empty() || _currentlyServicingRangeRequest);

    // Note that request is being serviced, if one was not before
    _currentlyServicingRangeRequest = true;
    _start = start;
    _end = end;
    _pieceRequests.clear();

    // << Do something more sophisticated later >>
    Q_ASSERT(_handle.is_valid());

    // Get torrent file
    boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfo = _handle.torrent_file();

    // We should always have metadata if a stream has been started
    Q_ASSERT(torrentInfo != NULL);

    // Figure which pieces where range [_start, _end] of file begins
    libtorrent::peer_request rStart = torrentInfo->map_file(_fileIndex, _start, 0);

    // Figure which pieces where range [_start, _end] of file ends
    libtorrent::peer_request rEnd = torrentInfo->map_file(_fileIndex, _end, 0);

    // Iterate piece indexes and setup piece requests
    _numberOfPiecesNotRead = rEnd.piece - rStart.piece + 1;

    for(int i = rStart.piece;i < rEnd.piece;i++) {

        // Check if valid copy of piece has been downloaded
        if(_handle.have_piece(i)) {

            // If so, we ask to read it,
            _handle.read_piece(i);

            // and add piece request
            _pieceRequests.push_back(PieceRequest(i, torrentInfo->piece_size(i), PieceRequest::Status::waiting_for_it_to_be_read, boost::shared_array<char>(NULL)));
        } else // otherwise, we have to just wait for it to be available
            _pieceRequests.push_back(PieceRequest(i, torrentInfo->piece_size(i), PieceRequest::Status::waiting_for_it_be_downloaded, boost::shared_array<char>(NULL)));
    }

    Q_ASSERT(_pieceRequests.size() == _numberOfPiecesNotRead);
}

void Stream::getStreamPieces(int start) {
    getStreamPieces(start, start + _defaultRangeLength);
}

void Stream::sendStream() {

   /**
   const QString & contentType,
   int start,
   int end,
   int total,
   const QVector<Piece> & pieces,
   int offsetInFirstPiece,
   int offsetInLastPiece
   */

    // * contentType = type of data, e.g. "video/mp4"
    // * start,end = what subrange of total underlying data stream is being transmitted
    // * total = full range of data stream
    // * pieces = actual data
    // * startOffsetInFirstPiece = Offset to start in first piece (inclusive)
    // * stopOffsetInLastPiece = Offset to stop in last piece (inclusive)


    /**
     * Figure out offsets in first and last pieces
     *
     *
     *
     *
     *

    // Offset to start in first piece (inclusive)
    int _startOffsetInFirstPiece;

    // Offset to stop in last piece (inclusive)
    int _stopOffsetInLastPiece;




    int firstPieceSize = _torrentInfo->piece_size(r.piece);

    // Treat a range which is within a single piece as an exception
    if(r.start + r.length <= firstPieceSize) { // NB: rangeSize > firstPieceSize does not test this

        _numberOfOutstandingPieces = 1;
        _stopOffsetInLastPiece = r.start + r.length - 1;
        _pieces.push_back(Piece(r.piece, firstPieceSize, false, boost::shared_array<char>(NULL)));

        // Request that piece is read
        emit pieceNeeded(r.piece);

    } else {

        int requiredDataInFirstPiece = firstPieceSize - r.start;

        // Figure out how many pieces in total are touched by range
        int numberOfPiecesAfterFirst = (r.length - requiredDataInFirstPiece) / _torrentInfo->piece_length();
        _numberOfOutstandingPieces = 1 + numberOfPiecesAfterFirst;

        for(int i = r.piece;i < _numberOfOutstandingPieces;i++) {

            // Get size of this piece, where
            // last piece may be shorter than rest, which are uniform
            int pieceSize = _torrentInfo->piece_size(i);

            // Crate piece object
            _pieces.push_back(Piece(i, pieceSize, false, boost::shared_array<char>(NULL)));

            // Request that piece is read
            emit pieceNeeded(i);
        }

        Q_ASSERT(_pieces.size() == _numberOfOutstandingPieces);

        // How much of range oveflows into last piece
        _stopOffsetInLastPiece = (r.length - requiredDataInFirstPiece) % _torrentInfo->piece_length();
    }
    */

    qDebug() << "sendDataRange"
             << contentType
             << ", #pieces=" << pieces.size()
             << "," << offsetInFirstPiece
             << "," << offsetInLastPiece;

    Q_ASSERT(!_pieceRequests.empty());
    Q_ASSERT(_start < _end);
    Q_ASSERT(_end < _total);

    // Build response header
    /**
     * http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html
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
    */

    // Length of actual data range being sent with this response
    int contentLength = end - start + 1;

    QString header = QString("HTTP/1.1 206 Partial Content\n") +
                     QString("Content-Range: bytes ") + QString::number(start) + QString("-") + QString::number(end) + QString("/") + QString::number(total) + QString("\n") +
                     QString("Accept-Ranges: bytes\n") +
                     QString("Content-Length: ") + QString::number(contentLength) + QString("\n") +
                     QString("Content-Type: ") + contentType + QString("\n") +
                     QString("\n"); // Extra new lin required

    // Write response header
    _socket->write(header.toUtf8());

    // Recomputed length of data sent, used for assert
    int recomputedContentLength = 0;

    // If we only have one piece, than just count what part of it is in range
    if(pieces.length() == 1) {

        // Its the same piece!
        Q_ASSERT(offsetInLastPiece > offsetInFirstPiece);

        // Add to length (which is really full length in this case)
        recomputedContentLength = offsetInLastPiece - offsetInFirstPiece + 1;

        // Get piece data
        const boost::shared_array<char> & data = pieces.front().data();

        // Write sub range of piece
        _socket->write(data.get() + offsetInFirstPiece, recomputedContentLength);

    } else {

        // First piece may, potentially, not be a full piece, but rather a postfix
        const Piece & piece = pieces.front();

        recomputedContentLength = piece.length() - offsetInFirstPiece;
        _socket->write(piece.data().get() + offsetInFirstPiece, recomputedContentLength);

        // Iterate rest of pieces, starting at second piece
        for(int i = 1;i < pieces.length();i++) {

            // Get piece
            const Piece & piece = pieces.front();

            // Treat last piece separately
            if(i == pieces.length() - 1) {

                // Last piece may, potentially, not be a full piece, but rather a prefix
                recomputedContentLength += offsetInLastPiece + 1;
                _socket->write(piece.data().get(), offsetInLastPiece + 1);

            } else {

                // Interior pieces are written in full
                recomputedContentLength += piece.length();
                _socket->write(piece.data().get(), piece.length());

            }
        }
    }

    Q_ASSERT(recomputedContentLength == contentLength);

    _socket->flush(); // Needed before change 86186 <== ?
}

QByteArray Stream::requestedPath() const {
    return _requestedPath;
}

libtorrent::torrent_handle Stream::handle() const {
    return _handle;
}
