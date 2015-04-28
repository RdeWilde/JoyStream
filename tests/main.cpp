#include <QApplication>

#include "TestLoggerManager.hpp"
#include "TestTxId.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //app.setAttribute(Qt::AA_Use96Dpi, true);

    TestLoggerManager tcTestLoggerManager;
    QTest::qExec(&tcTestLoggerManager, argc, argv);

    TestTxId tcTestTxId;
    QTest::qExec(&tcTestTxId, argc, argv);
}
