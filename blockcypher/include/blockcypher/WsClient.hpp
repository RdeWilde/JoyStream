#ifndef WSCLIENT_HPP
#define WSCLIENT_HPP

#include <QObject>
#include <QtWebSockets/QtWebSockets>
#include <event.hpp>
#include "TX.hpp"
#include <QStringList>

//websocket
//Please refer to BlockCypher documentation for further information about endpoints.
#define BLOCKCYPHER_WEBSOCKET_TESTNET3_ENDPOINT "wss://socket.blockcypher.com/v1/btc/test3"
#define TIMERINTERVALL 12000 // 12s



namespace BlockCypher {

//class QUrl;

class WSClient : public QObject {

    Q_OBJECT

public:
    explicit WSClient(QObject *parent = 0);

    //Add events to the active filter.
    void addEvent(const Event & e);

    void openConnection(void);
    void closeConnection(void);

signals:
    void connected(void);
    void disconnected(void);
    void ws_msgReceived(QJsonDocument);

    void txArrived(const TX &);

    //TODO: Not implemented.
    //void blockArrived(const Block &);

private:

    void wsError(QAbstractSocket::SocketError error);

    //Keep connection alive.
    void keepAlive(void);

    void wsConnected(void);
    void wsDisconnected(void);

    //Handles textMessageReceived signal from socket.
    void msgIncoming(QString msg);

    bool isConnected(void) {
        return ((client_socket.state() == QAbstractSocket::ConnectedState) ? true : false);
    }

    template<class T>
    void getelements(QList<T> l, const QJsonValue & val);
    bool probeNotification(QStringList,const QJsonObject &);

    QWebSocket client_socket;
    QUrl client_url;
    QTimer _timer;

};


} // end namespace BlockCypher
#endif // WSCLIENT_HPP
