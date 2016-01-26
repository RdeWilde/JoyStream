/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 26 2016
 */

#include <blockcypher/WebSocketClient.hpp>
#include <blockcypher/TX.hpp>

#include <common/Network.hpp>

#include <QObject>
#include <QAbstractSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

namespace BlockCypher {

    WebSocketClient::WebSocketClient(Coin::Network network)
        : webSocketEndpoint(endPoint(network)) {

        // Capture signals and use a
        QObject::connect(&_pingTimer, &QTimer::timeout, [this](){ sendPing(); });
        QObject::connect(&_webSocket, &QWebSocket::connected, [this](){ webSocketConnected(); });
        QObject::connect(&_webSocket, &QWebSocket::disconnected, [this](){ webSocketDisconnected(); });
        QObject::connect(&_webSocket, &QWebSocket::textMessageReceived, [this](QString s){ webSocketTextMessageArrived(s); });
        //QObject::connect(&_webSocket, &QWebSocket::error, [this](QAbstractSocket::SocketError e){ webSocketErrorOccured(e); });
        QObject::connect(&_webSocket, static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error), [this](QAbstractSocket::SocketError e){ webSocketErrorOccured(e); });

        // Start timer, is never stopped
        _pingTimer.start(PING_MS_INTERVAL);
    }

    const char * WebSocketClient::endPoint(Coin::Network network) {

        switch(network) {
            case Coin::Network::mainnet: return BLOCKCYPHER_MAINNET_SOCKET_ENDPOINT;
            case Coin::Network::testnet3: return BLOCKCYPHER_TESTNET3_SOCKET_ENDPOINT;
        }

        Q_ASSERT(false);
    }


    void WebSocketClient::connect() {

        if (!isConnected()) {

            _webSocket.open(webSocketEndpoint);

            // Send all events
            foreach( const Event & e, _addedEvents)
                sendEvent(e);

        } else
            throw std::runtime_error("Already connected.");
    }

    void WebSocketClient::disconnect() {
        _webSocket.close();
    }

    void WebSocketClient::addEvent(const Event & e) {

        // Check that we support this event type
        if(e.getEvent() == Event::Type::new_block)
            throw std::runtime_error("Unsupported event type.");

        // Add to list of events
        _addedEvents.append(e);

        // Try to send event if connected
        if(isConnected()) {

            QJsonValue json(e.toJson());

            _webSocket.sendTextMessage(json.toString());
        }
    }

    bool  WebSocketClient::isConnected() const {
        return _webSocket.state() == QAbstractSocket::ConnectedState;
    }

    void WebSocketClient::webSocketConnected() {
        emit connected();
    }

    void WebSocketClient::webSocketDisconnected() {

        _pingTimer.stop();

        emit disconnected();
    }

    void WebSocketClient::webSocketErrorOccured(QAbstractSocket::SocketError e) {
        emit error(e);
    }

    void WebSocketClient::webSocketTextMessageArrived(QString msg) {

        // Turn into JSON
        QJsonDocument doc = QJsonDocument::fromJson(msg.toLatin1());

        if(!doc.isObject())
            throw std::runtime_error("Receive non-JSON response.");

        QJsonObject JSON = doc.object();

        // Deduce what event type it corresponds to
        Event::Type eventType = Event::getPayloadType(JSON);

        // if we don't support it, then throw exception
        if(eventType == Event::Type::new_block)
            throw std::runtime_error("Event type not supported.");

        // Should we do super slow ASSERTS: we have installed event with same type

        // Turn into TX object
        TX tx(JSON);

        // Emit signal
        emit txArrived(tx, eventType);
    }

    void WebSocketClient::sendPing(void) {

        // Check that we are actually connected
        if(!isConnected())
            return;

        // Send ping even JSON, must be sent at least every 20s
        // http://dev.blockcypher.com/?shell#using-websockets
        _webSocket.sendTextMessage("{ \"event\": \"ping\" }");

    }

    void WebSocketClient::sendEvent(const Event & e) {

        // This private routine should never be called if
        // we are not connected
        Q_ASSERT(isConnected());

        // Send JSON version of event
        QJsonValue eventJSON(e.toJson());
        _webSocket.sendTextMessage(eventJSON.toString());
    }

}
