#include "TestStream.hpp"

#include <QtTest/QtTest>

TestStream::TestStream(QObject *parent)
    : QObject(parent) {

}

void TestStream::getRange(int start, int end) {

}

void TestStream::getStart(int start) {

}

void TestStream::processError(Stream::Error errorOccured) {

}

void TestStream::initTestCase() {
    //qDebug("initTestCase()");
}

void TestStream::cleanupTestCase() {
    //qDebug("cleanupTestCase()");
}

void TestStream::init() {
    //qDebug("init()");
}

void TestStream::cleanup() {
    //qDebug("cleanup()");
}

void TestStream::dummyCase() {
    QVERIFY(1 == 1);
}

// QTEST_MAIN:
// Generates a main routine which runs all private slot
// test routines on TestLoggerManager

//QTEST_MAIN(TestStreamingServer)
