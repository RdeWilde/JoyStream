#ifndef HTTP_CONNECTION_HANDLER_HPP
#define HTTP_CONNECTION_HANDLER_HPP

#include <QObject>
#include <QMultiMap>

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

        // Stream ended
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

    // Sends range of data
    void sendDataRange(int start, const char * data);

    // Closes socket
    // think more about how to do this properly to avoid
    // leaks void close();

signals:

    // The given byte range of the data stream was requested
    void rangeRequested(int start, int end);

    // Given error occured, stream has been closed
    // and will be promptly deleted.
    void errorOccured(Error errorOccured);

private:

    void processRequest();

    // Tries to read request line from socket
    void readRequestLine();

    // Tries to read
    void readRequestHeaders();

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
};

#endif // HTTP_CONNECTION_HANDLER_HPP
