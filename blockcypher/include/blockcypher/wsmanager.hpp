#ifndef WSMANAGER_HPP
#define WSMANAGER_HPP

#include <QObject>
#include <QMap>
#include "wsclient.hpp"
#include "TX.hpp"


using namespace BlockCypher;


class WsManager : public QObject {
    Q_OBJECT
public:
    explicit WsManager(QObject *parent = 0);
    void runTests();

signals:
    void connected(void);
    void disconnected(void);
    void openConnection(void);
    void closeConnection(void);
    void msgReceived(QJsonDocument);

public slots:
    void initConnection(void);
    void endConnection(void);
    void netConnected(void);
    void netDisconnected(void);
    void newMsgReceived(QJsonDocument);
    void manager_txArrived(const TX &);

private:
    WSClient ws_client;

    //WSClient::WebsocketSignalHandler _handler;


};



#endif // WSMANAGER_HPP
