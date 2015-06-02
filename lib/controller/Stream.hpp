#ifndef HTTP_CONNECTION_HANDLER_HPP
#define HTTP_CONNECTION_HANDLER_HPP

#include <libtorrent/torrent_handle.hpp>

#include <QObject>
#include <QMultiMap>
#include <QVector>

#ifndef Q_MOC_RUN
#include <boost/shared_array.hpp>
#endif Q_MOC_RUN

class QTcpSocket;
class Controller;

class Stream : public QObject
{
    Q_OBJECT

public:

    // Status associated with processing socket
    enum class SocketProcessingState {

        // Initial state of stream
        // the only circumstance under which _requestedPath
        // can be set, in the future it will only be compared.
        WaitingForFirstRequestLine,

        // We have yet to receive a full request line
        ReadingRequestLine,

        // We have yet to read the end of all headers in a request
        ReadingRequestHeaders,

        // Stream ended: Why this?
        //Done,

        // Socket closed by peer
        SocketClosedByPeer,

        // Socket was closed by us,
        // check error state to see if it was graceful
        SocketClosedByUs
    };

    // Error states
    enum class Error {

        // Client input was not a GET method call
        NotAGET,

        // Request line was longer than MAX_REQUEST_LINE_LENGTH
        RequestLineLengthToLong,

        // It must atleast be \x where x is one or more chars
        RequestedPathToShort,

        // Client sent a new request path
        ChangedRequestPath,

        // Last header line read is missing a colon
        HeaderLineMissingColon,

        // Could not parse range header line
        InvalidRangeHeaderLine,

        // Range was not compatible with underlying content
        InvalidRangeRequested,

        // Provided path did not have a valid info hash,
        // that is one corresponding to a torrent added to controller
        InvalidInfoHash,
    };

    // A request for a piece
    class PieceRequest {

    public:

        // Status of a request
        enum class Status {

            // libtorrent::finished_piece_alert
            waiting_for_it_be_downloaded,

            // libtorrent::read_piece_alert
            waiting_for_it_to_be_read,

            // received libtorrent::read_piece_alert and saved
            ready_in_buffer,

            // ther was some sort of error in libtorrent::read_piece_alert
            read_piece_failed
        };

        // Default constructor for QVector
        PieceRequest();

        PieceRequest(int index, int length, Status status, const boost::shared_array<char> & buffer);

        // Getters and setters
        int index() const;
        void setIndex(int index);

        int length() const;
        void setLength(int length);

        Status status() const;
        void setStatus(Status status);

        boost::shared_array<char> buffer() const;
        void setBuffer(const boost::shared_array<char> & buffer);

    private:

        // Index of piece
        int _index;

        // Byte length of piece
        int _length;

        // Status of request
        Status _status;

        // Data, length is in _length
        boost::shared_array<char> _buffer;
    };

    // Constructor
    Stream(QTcpSocket * socket, Controller * controller);

    // Destructor
    ~Stream();

    // Given piece was read
    void pieceRead(const boost::shared_array<char> & buffer, int pieceIndex, int size);

    // Given piece was downloaded and checked
    void pieceDownloaded(int piece);

    // Getters
    QByteArray requestedPath() const;

    libtorrent::sha1_hash infoHash() const;

public slots:

    // Try to read socket and parse data into request
    void readSocket();

    // Socket has been closed
    void socketWasClosed();

private:

    // Tries to read request line from socket
    void readAndProcessRequestLineFromSocket(const QByteArray & line);

    // Tries to read request header lines from socket, returns
    // whether a full request could be completed, results are in _headers
    bool readRequestHeaderLineFromSocket(const QByteArray & line);

    // Simple splitting utility used for processing request liness
    static QPair<QByteArray, QByteArray> splitInHalf(QByteArray data, char c, bool & ok);

    // Processes contents of _headers as if they represent a full request
    void processRequest();

    // Sends error to client, closes connection
    // and schedules deletion of stream from event loop
    void sendErrorToPeerAndEndStream(Error errorOccured);

    // Tries to get pieces within given file range
    void getStreamPieces(int start, int end);

    // Tries to get pieces starting at given file offset for standard range size (_defaultRangeLength)
    void getStreamPieces(int start);

    // Sends response with data over socket to client
    void sendStream() const;

    // Controller to which stream corresponds
    Controller * _controller;

    // Sate of handler
    SocketProcessingState _socketProcessingState;

    // Socket for connection
    QTcpSocket * _socket;

    // Handle of torrent to which this stream corresponds
    libtorrent::torrent_handle _handle;

    // Info hash for torrent which has been requested
    // Is deduced from the requested path supplied by peer,
    // which should never change. The reason _handle, which
    // can generate an info hash, is not enough,
    // is because the handle may have expired, or not yet been
    // established.
    libtorrent::sha1_hash _infoHash;

    /**
     * Client socket data for most recent request
     * ===================
     */

    // Contains path requested, is only valid
    // when _state != ReadingRequest
    QByteArray _requestedPath;

    // Maps capitalized field name to set of values observed
    // for this field.
    // More on this: https://tools.ietf.org/html/rfc7230#section-3.2.2
    QMultiMap<QByteArray, QByteArray> _headers;

    /**
     * Piece reading related state
     * ===================
     */
    // The index of the *single* in the torrent which can be streamed
    int _fileIndex;

    // Total byte size of file
    int _totalLengthOfFile;

    // Content type of content
    QString _contentType;

    // The number of bytes
    int _defaultRangeLength;

    ////
    //// Everything below here is computed from the last request
    //// and is kept around.
    ////

    // Most recent request
    int _start, _end;

    // Offset in first piece to which _start corresponds
    int _startOffsetInFirstPiece;

    // Offset in last piece to which _end corresponds
    int _stopOffsetInLastPiece;

    // The number of pieces for which we have not yet read data,
    // that is where status of piece
    // is != PieceRequest::Status::ready_in_buffer
    int _numberOfPiecesNotRead;

    // Piece requests for current requested range
    QVector<PieceRequest> _pieceRequests;
};

#endif // HTTP_CONNECTION_HANDLER_HPP
