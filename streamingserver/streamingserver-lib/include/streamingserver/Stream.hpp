#ifndef HTTP_CONNECTION_HANDLER_HPP
#define HTTP_CONNECTION_HANDLER_HPP

#include <QObject>
#include <QMultiMap>

#ifndef Q_MOC_RUN
#include <boost/shared_array.hpp>
#endif Q_MOC_RUN

class QTcpSocket;

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
    Stream(QTcpSocket * socket, QObject * parent = 0);

    // Destructor
    ~Stream();

    // Getters
    QByteArray requestedPath() const;


public slots:

    // Try to read socket and parse data into request
    void readSocket();

    // Sends data
    void sendDataRange(const QString & contentType,
                       const QList<const boost::shared_array<char> > & data,
                       int offsetInFirstChunk,
                       int offsetInLastArray);

    // A request starting at given position was invalid
    void invalidRangeRequested(int start);

signals:

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
