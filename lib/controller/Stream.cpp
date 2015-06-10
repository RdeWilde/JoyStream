#include "Stream.hpp"

/**
 * Stream::PieceRequest
 */

Stream::PieceRequest::PieceRequest() {

}

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

    // Do nothing if no range request is being serviced, or this piece is not in that range
    if(_pieceRequests.empty() ||
       pieceIndex < _pieceRequests.front().index() ||
       pieceIndex > _pieceRequests.last().index())
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

    Q_ASSERT(_numberOfPiecesNotRead >= 0);

    // Do we have all pieces now?
    if(_numberOfPiecesNotRead == 0) {

        // Send response to client
        sendStream();

        // Clear out requests
        _pieceRequests.clear();
    }
}

void Stream::pieceDownloaded(int pieceIndex) {

    // Do nothing if no range request is being serviced, or this piece is not in that range
    if(_pieceRequests.empty() ||
       pieceIndex < _pieceRequests.front().index() ||
       pieceIndex > _pieceRequests.last().index())
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

    // Whether socket ended in one full request
    bool socketEndedInFullRequest = false;

    // Try to read all full lines available
    while(_socket->canReadLine() &&
          _socketProcessingState != SocketProcessingState::SocketClosedByPeer &&
          _socketProcessingState != SocketProcessingState::SocketClosedByUs) {

        // We have to reset this, incase multiple requests were ready, e.g. with fast seeking
        socketEndedInFullRequest = false;

        // Read line
        QByteArray line = _socket->readLine();

        // Should hold by loop condition
        Q_ASSERT(line.length() > 0);

        if(_socketProcessingState == SocketProcessingState::WaitingForFirstRequestLine ||
           _socketProcessingState == SocketProcessingState::ReadingRequestLine)
            readAndProcessRequestLineFromSocket(line);
        else if(_socketProcessingState == SocketProcessingState::ReadingRequestHeaders)
            // Even if a full request was found, it may not be processed if there is another one in the pipe
           socketEndedInFullRequest = readRequestHeaderLineFromSocket(line);
        else
            Q_ASSERT(false); // all cases should have been covered
    }

    // We dont have a full line, due to loop stop condition,
    // yet socket has more than the line length limit, then end stream
    if (_socket->bytesAvailable() > MAX_REQUEST_LINE_LENGTH) {

        sendErrorToPeerAndEndStream(Error::RequestLineLengthToLong);
        return;
    }

    // If socket ended in one full request, and nothing more then
    if(socketEndedInFullRequest) {

        // process it
        processRequest();

        // and reset for next request
        _socketProcessingState = SocketProcessingState::ReadingRequestLine;
        _headers.clear();
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

    // If we registered to begin with, then unregister with controller,
    if(_socketProcessingState != SocketProcessingState::WaitingForFirstRequestLine)
        _controller->unRegisterStream(this);

    // Delete *this stream later from event loop
    deleteLater();
}

void Stream::readAndProcessRequestLineFromSocket(const QByteArray & line) {

    // Make sure we are supposed to be reading request line
    Q_ASSERT((_socketProcessingState == SocketProcessingState::WaitingForFirstRequestLine && _requestedPath.size() == 0) ||
             (_socketProcessingState == SocketProcessingState::ReadingRequestLine && _requestedPath.size() != 0));

    // Split to recover method used
    QList<QByteArray> requestLineSections = line.split(' ');

    // We only support GET
    if (requestLineSections[0].toUpper() != QByteArrayLiteral("GET")) {

        qDebug() << "Not a GET method call:" << requestLineSections[0];
        sendErrorToPeerAndEndStream(Error::NotAGET);
        return;
    } else if(requestLineSections[1].size() < 2) {

        qDebug() << "Requested path to short:" << requestLineSections[1];
        sendErrorToPeerAndEndStream(Error::RequestedPathToShort);
        return;
    }

    // Remove first symbol, which should be "/"
    const QByteArray newRequestedPath = requestLineSections[1].remove(0,1);

    // Save requested path if this is the first time we see it
    if(_socketProcessingState == SocketProcessingState::WaitingForFirstRequestLine) {

        qDebug() << "Got request for:" << newRequestedPath;

        Q_ASSERT(_requestedPath.size() == 0);
        Q_ASSERT(_pieceRequests.empty());

        // Save requested path
        _requestedPath = newRequestedPath;

        // Decode hex string
        QByteArray hexDecodedInfoHash = QByteArray::fromHex(_requestedPath);

        // Must be 20 bytes
        if(hexDecodedInfoHash.length() != 20) {

            qDebug() << "Info hash is not 20 bytes:" << _requestedPath;
            sendErrorToPeerAndEndStream(Error::InvalidInfoHash);
            return;
        }

        // Create info hash
        _infoHash = libtorrent::sha1_hash(hexDecodedInfoHash.constData());

        // Try to get torrent handle and subscribe to piece events if this request is valid
        _handle = _controller->registerStream(this);

        // End if it was not valid
        if(!_handle.is_valid()) {

            qDebug() << "Info hash does not match any added torrent:" << _requestedPath;
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

        _defaultRangeLength = torrentInfo->piece_length() * 10; //
        _fileIndex = 0;

        // Get file entry
        libtorrent::file_entry fileEntry = torrentInfo->file_at(_fileIndex);

        // Save size of file
        _totalLengthOfFile = fileEntry.size;

        // Detect content type
        QString path = QString::fromStdString(fileEntry.path);

        if(path.contains(QString("mp3"), Qt::CaseInsensitive))
            _contentType = "audio/mpeg";
        else if(path.contains(QString("mp4"), Qt::CaseInsensitive))
            _contentType = "video/mp4";
        else {

            qDebug() << "Cannot deduce content-type from file:" << path;
            Q_ASSERT(false);
        }

    } // If its not the first time, make sure its the same path, otherwise the peer is misbehaving
    else if(_requestedPath != newRequestedPath) {

        qDebug() << "Changed request path from" << _requestedPath << "to" << newRequestedPath;
        sendErrorToPeerAndEndStream(Error::ChangedRequestPath);
        return;
    }

    // Update state
    _socketProcessingState = SocketProcessingState::ReadingRequestHeaders;
}

bool Stream::readRequestHeaderLineFromSocket(const QByteArray & line) {

    Q_ASSERT(_socketProcessingState == SocketProcessingState::ReadingRequestHeaders);
    Q_ASSERT(_requestedPath.size() != 0);

    // If all headers have now been read, then process full request
    if (line == QByteArrayLiteral("\r\n") || line == QByteArrayLiteral("\n"))
        return true;

    // Split header line to recover name and value
    bool ok;
    QPair<QByteArray, QByteArray> splittedHeaderLine = splitInHalf(line, ':', ok);

    // Was splitting possible?
    if(ok) {

        qDebug() << splittedHeaderLine.first << ':'<< splittedHeaderLine.second;

        // Insert into header map
        _headers.insert(splittedHeaderLine.first.toUpper(), splittedHeaderLine.second.trimmed());

    } else {

        qDebug() << "Header line is missing colon:" << line;
        sendErrorToPeerAndEndStream(Error::HeaderLineMissingColon);
    }

    return false;
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

        // Remove carrige return and newline at end of line
        returnValue.second = returnValue.second.trimmed();
    }

    return returnValue;
}


void Stream::processRequest() {

    Q_ASSERT(_socketProcessingState == SocketProcessingState::ReadingRequestHeaders);
    Q_ASSERT(_requestedPath.size() != 0);

    // Try to recover range field
    QByteArray range = QByteArrayLiteral("RANGE");

    // If range exists, try to use it
    if(_headers.contains(range)) {

        // Lets just assume there is only one range field for now
        QList<QByteArray> headers = _headers.values(range);

        // Lets just support one range field for now
        Q_ASSERT(headers.size() == 1);
        QByteArray rangeValue = headers.front();

        // Remove "bytes"
        rangeValue.replace("bytes=","");

        // Split range into start and end, if possible.
        bool ok;
        QPair<QByteArray, QByteArray> splittedRangeValue = splitInHalf(rangeValue, '-', ok);

        // Was it possible?
        if(ok) {

            bool couldConvertStartOfRange;
            int start = splittedRangeValue.first.toInt(&couldConvertStartOfRange);

            if(!couldConvertStartOfRange) {

                qDebug() << "Could not recover start of range:" << splittedRangeValue.first;

                sendErrorToPeerAndEndStream(Error::InvalidRangeHeaderLine);

            } else if(splittedRangeValue.second == QByteArrayLiteral("")) // If end of range is empty, then just use start
                getStreamPieces(start);
            else {

                bool couldConvertEndOfRange;
                int end = splittedRangeValue.second.toInt(&couldConvertEndOfRange);

                // Could actual range values be recovered?
                if(couldConvertEndOfRange) {

                    qDebug() << "Requesting range [" << start << "," << end << "]";

                    // Send range request notification
                    getStreamPieces(start, end);

                } else {

                    qDebug() << "Could not convert en of range:" << splittedRangeValue.second;

                    sendErrorToPeerAndEndStream(Error::InvalidRangeHeaderLine);
                }
            }

        } else {

            qDebug() << "Range header missing - :" << rangeValue;

            sendErrorToPeerAndEndStream(Error::InvalidRangeHeaderLine);
        }

    } else
        // Just dump first part of file
        getStreamPieces(0);
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

    QByteArray body("<html><body>Error</body></html>");

    _socket->write(headers.arg(400).arg(body.length()).toUtf8());
    _socket->write(body);
    _socket->flush(); // Needed before change 86186

    // Close socket
    _socket->close();

    // Delete *this stream later from event loop
    deleteLater();
}

void Stream::getStreamPieces(int start, int end) {

    // Check that the requested range is valid
    if(start < 0 || end >= _totalLengthOfFile || end <= start) {

        qDebug() << "Invalid range request [" << start << "," << end << "]";

        // End it
        sendErrorToPeerAndEndStream(Error::InvalidRangeRequested);

        return;
    }

    /**
     * Note that any previously active request will simply
     * be overwritten below
     */

    if(!_pieceRequests.empty()) {

        qDebug() << "Dropping currently active request for [" << _start << "," << _end << "]";
        _pieceRequests.clear();
    }

    _start = start;
    _end = end;

    // << Do something more sophisticated later >>
    Q_ASSERT(_handle.is_valid());

    // Get torrent file
    boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfo = _handle.torrent_file();

    // We should always have metadata if a stream has been started
    Q_ASSERT(torrentInfo != NULL);

    // Figure which pieces where range [_start, _end] of file begins
    libtorrent::peer_request rStart = torrentInfo->map_file(_fileIndex, _start, 0);

    // Save offset needed later when writing stream to socket
    _startOffsetInFirstPiece = rStart.start;

    // Figure which pieces where range [_start, _end] of file ends
    libtorrent::peer_request rEnd = torrentInfo->map_file(_fileIndex, _end, 0);

    // Save offset needed later when writing piece data to socket
    _stopOffsetInLastPiece = rEnd.start;

    // Iterate piece indexes and setup piece requests
    _numberOfPiecesNotRead = rEnd.piece - rStart.piece + 1;

    bool atLeastOnePieceIsNotDownloaded = false;

    for(int i = rStart.piece;i <= rEnd.piece;i++) {

        // Check if valid copy of piece has been downloaded
        if(_handle.have_piece(i)) {

            // If so, we ask to read it,
            _handle.read_piece(i);

            // and add piece request
            _pieceRequests.push_back(PieceRequest(i, torrentInfo->piece_size(i), PieceRequest::Status::waiting_for_it_to_be_read, boost::shared_array<char>(NULL)));

        } else {

            // otherwise, we have to just wait for it to be available
            _pieceRequests.push_back(PieceRequest(i, torrentInfo->piece_size(i), PieceRequest::Status::waiting_for_it_be_downloaded, boost::shared_array<char>(NULL)));

            // if this is the first piece not downloaded, then tell controller to get plugin to download from new this position
            if(!atLeastOnePieceIsNotDownloaded) {

                atLeastOnePieceIsNotDownloaded = true;

                qDebug() << "Relocating download point since the following pieces has not been downloaded" << i;

                // Ask controller to relocate buyer plugin download
                _controller->changeDownloadingLocationFromThisPiece(torrentInfo->info_hash(), i);
            }
        }
    }

    Q_ASSERT(_pieceRequests.size() == _numberOfPiecesNotRead);
}

void Stream::getStreamPieces(int start) {
    getStreamPieces(start, start + _defaultRangeLength - 1);
}

void Stream::sendStream() const{

    qDebug() << "sendStream";

    Q_ASSERT(!_pieceRequests.empty());
    Q_ASSERT(_start < _end);
    Q_ASSERT(_end < _totalLengthOfFile);

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
    int streamLength = _end - _start + 1;

    QString header = QString("HTTP/1.1 206 Partial Content\n") +
                     QString("Content-Range: bytes ") + QString::number(_start) + QString("-") + QString::number(_end) + QString("/") + QString::number(_totalLengthOfFile) + QString("\n") +
                     QString("Accept-Ranges: bytes\n") +
                     QString("Content-Length: ") + QString::number(streamLength) + QString("\n") +
                     QString("Content-Type: ") + _contentType + QString("\n") +
                     QString("\n"); // Extra new lin required

    // Write response header
    _socket->write(header.toUtf8());

    // Write stream by iterating pieces
    // Requires awarness of first and last piece edge cases,
    // so it gets a bit involved

    // Torrent file needed to get piece offsets in first and last piece

    Q_ASSERT(_handle.is_valid()); // more clever here in the future

    boost::intrusive_ptr<libtorrent::torrent_info const> torrentInfo =_handle.torrent_file();

    // We should always have metadata if a stream has been started
    Q_ASSERT(torrentInfo != NULL);

    // counter used for invariant later
    int numberOfBytesWritten = 0;
    for(int i = 0;i < _pieceRequests.size();i++) {

        // Get piece request
        const PieceRequest & pieceRequest = _pieceRequests[i];

        // Handle first piece as special case,
        // as it likely not be written fully
        if(i == 0) {

            // How much is left of piece from start offset to end of piece
            int sizeOfRestOfPiece = pieceRequest.length() - _startOffsetInFirstPiece;

            // How much should be written of piece
            // It may be that we should not write full piece,
            // if the requested range is subset of a single piece!
            int sizeOfPiecePostfixToWrite = (streamLength > sizeOfRestOfPiece) ? sizeOfRestOfPiece : streamLength;

            // Send subrange of piece
            _socket->write(pieceRequest.buffer().get() + _startOffsetInFirstPiece, sizeOfPiecePostfixToWrite);

            // Count bytes written
            numberOfBytesWritten += sizeOfPiecePostfixToWrite;

        } else {

            // How much of prefix of piece should be writte:
            // for last piece, use stored offset, for internal piece write full piece
            int sizeOfPiecePrefixToWrite = (i == _pieceRequests.size() - 1) ? _stopOffsetInLastPiece + 1 : pieceRequest.length();

            // Send subrange of piece
            _socket->write(pieceRequest.buffer().get(), sizeOfPiecePrefixToWrite);

            // Count bytes written
            numberOfBytesWritten += sizeOfPiecePrefixToWrite;
        }
    }

    Q_ASSERT(numberOfBytesWritten == streamLength);

    _socket->flush(); // Needed before change 86186 <== ?
}

QByteArray Stream::requestedPath() const {
    return _requestedPath;
}

libtorrent::sha1_hash Stream::infoHash() const  {
    return _infoHash;
}
