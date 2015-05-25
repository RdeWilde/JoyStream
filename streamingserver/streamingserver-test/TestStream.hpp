#ifndef TEST_STREAM_HPP
#define TEST_STREAM_HPP

#include <QObject>

#include "streamingserver/Stream.hpp"

class TestStream : public QObject
{
    Q_OBJECT
public:

    TestStream(QObject *parent = 0);

signals:

public slots:

    /**
     * Used to receive incoming signals from streams
     */

    // For Stream::rangeRequested(int start, int end)
    void getRange(int start, int end);

    // For Stream::startRequested(int start)
    void getStart(int start);

    // For Stream::errorOccured(Error errorOccured)
    void processError(Stream::Error errorOccured);

private slots:

    // Will be called before the first testfunction is executed.
    void initTestCase();

    // Will be called after the last testfunction was executed.
    void cleanupTestCase();

    // Will be called before each testfunction is executed.
    void init();

    // Will be called after every testfunction.
    void cleanup();

    /**
     * Actual test cases
     */

    // Actual tests
    void dummyCase();
};

#endif // TEST_STREAM_HPP
