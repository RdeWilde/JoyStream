#ifndef HTTP_CONNECTION_HANDLER_HPP
#define HTTP_CONNECTION_HANDLER_HPP

#include <QObject>
#include <QMultiMap>

#include "Piece.hpp"

class QTcpSocket;
class StreamingServer;

class Stream : public QObject
{
    Q_OBJECT

public:

    // Handler state
    enum class State {

        // Initial state of stream
        // the only circumstance under which _requestedPath
        // can be set, in the future it will only be compared.
        WaitingForFirstRequestLine,

        // We have yet to receive a full request line
        ReadingRequestLine,

        // We have yet to read the end of all headers in a request
        ReadingRequestHeaders,

        // Stream ended: Why this?
        Done,

        // Stream is in error state, and
        // details of error are communicated
        // by error signal
        Error
    };

    // Different errors which may occur
    enum class Error {

        // Client input was not a GET method call
        NotAGET,

        // Request line was longer than MAX_REQUEST_LINE_LENGTH
        RequestLineLengthToLong,

        // Client sent a new request path
        ChangedRequestPath,

        // Last header line read is missing a colon
        HeaderLineMissingColon,

        // Could not parse range header line
        InvalidRangeHeaderLine,

        // Range was not compatible with underlying content
        InvalidRangeRequested

    };

    // Constructor
    Stream(QTcpSocket * socket, QObject * parent);

    // Destructor
    ~Stream();

    // Getters
    QByteArray requestedPath() const;

public slots:

    // Try to read socket and parse data into request
    void readSocket();

    // Sends response with data over socket to client
    // * contentType = type of data, e.g. "video/mp4"
    // * start,end = what subrange of total underlying data stream is being transmitted
    // * total = full range of data stream
    // * pieces = actual data
    // * offsetInFirstPiece = offset to start writing from in first piece
    // * offsetInLastPiece
    void sendDataRange(const QString & contentType,
                       int start,
                       int end,
                       int total,
                       const QVector<Piece> & pieces,
                       int offsetInFirstPiece,
                       int offsetInLastPiece);

    // A request starting at given position was invalid
    void invalidRangeRequested(int start);

signals:

    // Client announced requested path for the first time, which should
    // never change for subsequent requests. This signal should be connected
    // using Qt::DirectConnection, since this information has to be
    // processed synchronously to allow recipient to catch *first* request.
    void requestedPathAnnounced(const Stream * handler, const QByteArray & requestedPath);

    // The given byte range of the data stream was requested
    void rangeRequested(int start, int end);

    // A chunk starting at given position has been requested, but no spesific end,
    // Its up to slot recipient to choose how big of a range to provide
    void startRequested(int start);

    // Given error occured, stream has been closed
    // and will be promptly deleted.
    void errorOccured(Error errorOccured);

private:

    // Processes contents of _headers as if they
    // represent a full request
    void processRequest();

    // Tries to read request line from socket
    void readRequestLine();

    // Tries to read
    void readRequestHeaders();

    // Sends given response with given error code to client
    void sendError(int code);

private:

    // Sate of handler
    State _state;

    // Socket for connection
    QTcpSocket * _socket;

    // Sends error signal, sends error to client, closes connection
    // and schedules deletion of *this from event loop
    void endOnError(Error errorOccured);

    /**
     * Below is data associated with most recent request being
     * parsed and processed.
     */

    // Contains path requested, is only valid
    // when _state != ReadingRequest
    QByteArray _requestedPath;

    // Maps capitalized field name to set of values observed
    // for this field.
    // More on this: https://tools.ietf.org/html/rfc7230#section-3.2.2
    QMultiMap<QByteArray, QByteArray> _headers;

    // Start of most recently requested range.
    // This is used to know whether to relay data ranges to the client,
    // as they may have expired.
    // We only keep track of start because not all requests
    // from client will include and end of range.
    int _mostRecentlyRequestedStartOfRange;

    // Simple splitting utility used for processing request line
    QPair<QByteArray, QByteArray> splitInHalf(QByteArray data, char c, bool & ok);
};

#endif // HTTP_CONNECTION_HANDLER_HPP
