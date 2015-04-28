#ifndef TEST_LOGGER_MANAGER_HPP
#define TEST_LOGGER_MANAGER_HPP

#include <QtTest/QtTest>

class TestLoggerManager : public QObject
{
    Q_OBJECT

private slots:

    // Will be called before the first testfunction is executed.
    void initTestCase();

    // Will be called after the last testfunction was executed.
    void cleanupTestCase();

    // Will be called before each testfunction is executed.
    void init();

    // Will be called after every testfunction.
    void cleanup();

    // Actual tests
    void dummyCase();
};

#endif // TEST_LOGGER_MANAGER_HPP
