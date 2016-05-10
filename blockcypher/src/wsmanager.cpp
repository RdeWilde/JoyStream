#include <blockcypher/WsClient.hpp>
#include <blockcypher/wsmanager.hpp>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QTime>


using namespace BlockCypher;

WsManager::WsManager(QObject *parent) : QObject(parent) {
    connect(&ws_client,SIGNAL(connected()),this,SLOT(netConnected()));
    connect(&ws_client,SIGNAL(disconnected()),this,SLOT(netDisconnected()));
    connect(&ws_client,SIGNAL(ws_msgReceived(QJsonDocument)),this,SLOT(newMsgReceived(QJsonDocument)));
    connect(&ws_client,SIGNAL(txArrived(const TX &)),this,SLOT(manager_txArrived(const TX &)));
}

void WsManager::initConnection(void) {
    ws_client.openConnection();
}

void WsManager::endConnection(void) {
    ws_client.closeConnection();
}

void WsManager::netConnected(void) {
    emit connected();
}

void WsManager::netDisconnected(void) {    
    emit disconnected();
}


void WsManager::newMsgReceived(QJsonDocument msg) {
    qDebug() << "WSManager::newMsgReceived\n" << msg;
    emit msgReceived(msg);
}


void WsManager::manager_txArrived(const TX &incoming_tx) {

    //We could print everything here to check that everything is stored correctly..

    qDebug() << "WsManager::manager_txArrived -> TX arrived: " << incoming_tx.getHash();

}


void WsManager::runTests() {

    //Jan 9th 2016: Note, These tests are not final and not very organized at the moment.

    //TODO: Test code should be moved to JoyStream/test/blockcypher
    //TODO: When you do port this over to the proper QTest framework,
    //TODO: do use real Q_VERIFY calls, and avoid doing print outs or
    //TODO: any sort of undeterministic looping in the tests.
    //TODO: This is a good start, but tests should in general not be
    //TODO: part of the implementation. The way it is done is to create
    //TODO: a class which inherits from QTest (http://doc.qt.io/qt-5/qtest.html),
    //TODO: look at the existing tests for guidance on how to do this

    // // Just add a slight delay, so the socket gets set up before the tests start.
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    // // 


    qDebug() << "Debug: WsManager::runTests : Running tests in WsManager";

    qDebug() << "Debug: WsManager::runTests : Setting up Event object";
    //Set up Event object.
    Event ev;

    ev.setEvent("tx-confirmation");
    ev.setAddress("n1p2k3yqWHFSWXQt3pNFZVPJ8rU4SQYAgG");
    ev.setConfirmations(1);

    //Testing constructor for tx-confirmation event.
    Event ev_test("tx-confirmation","nAAAAAyqWHFSWXQt3pNFZVPJ8rU4SQYAgG",1);
    Event ev_testtt("tx-confirmation","nAAAAAyqWHFSWXQt3pNFZVPJ8rU4SQYAgG",1,"dummytoken");


    Event ev_test4("tx-confirmation",0.88, "nAAAAAyqWHFSWXQt3pNFZVPJ8rU4SQYAgG");

    Event ev_test2("new-block");
    QJsonObject test1 = ev_test2.toJson();
    QJsonDocument doc1(test1);
    QString strJson1(doc1.toJson(QJsonDocument::Compact));
    qDebug() << "Debug: WsManager::runTests : QJson: " << strJson1;

    Event ev2;
    ev2.setEvent("new-block");

    qDebug() << "Debug: WsManager::runTests : Event object toJson test.";
    QJsonObject test = ev.toJson();
    QJsonDocument doc(test);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    qDebug() << "Debug: WsManager::runTests : QJson: " << strJson;

    //Add to filter
    qDebug() << "Debug: WsManager::runTests : Add to filter.";
    ws_client.addEvent(ev);  // tx-confirmation
    ws_client.addEvent(ev2); // new-block

}



