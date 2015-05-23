#include <QCoreApplication>

#include "TestLoggerManager.hpp"
#include "TestTxId.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    TestLoggerManager tcTestLoggerManager;
    QTest::qExec(&tcTestLoggerManager, argc, argv);

    TestTxId tcTestTxId;
    QTest::qExec(&tcTestTxId, argc, argv);
}
