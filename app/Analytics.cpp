/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Saturday 7 November 2015
 */

#include <Analytics.hpp>

#include <core/controller/Controller.hpp>
#include <core/controller/SellerPeerPluginViewModel.hpp>
#include <core/controller/BuyerTorrentPluginViewModel.hpp>
#include <core/controller/SellerTorrentPluginViewModel.hpp>
#include <core/extension/BuyerTorrentPlugin.hpp>
#include <core/extension/SellerTorrentPlugin.hpp>
#include <core/extension/SellerPeerPlugin.hpp>

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

QString Analytics::_defaultAnalyticsHost = DEFAULT_ANALYTICS_HOST;
quint64 Analytics::_defaultPingMsInterval = DEFUALT_PING_MS_INTERVAL;

Analytics::Analytics(QNetworkAccessManager * manager, const QString & host)
    : _manager(manager)
    , _host(host)
    , _started(false)
    , _numberOfAlivePings(0)
    , _numberOfPaidDownloadsStarted(0) {

    // Capture timer events
    QObject::connect(&_pingTimer, &QTimer::timeout, this, &Analytics::sendAlivePing);

    // Capture finished network event
    QObject::connect(_manager, &QNetworkAccessManager::finished, this, &Analytics::finished);
}

void Analytics::monitor(Controller * controller) {

    QObject::connect(controller, &Controller::addedTorrent, this, &Analytics::paidDownloadStarted);
}

QUrl Analytics::startEndPoint() const {
    return QUrl("http://" + _host + "/start");
}

QUrl Analytics::pingEndPoint() const {
    return QUrl("http://" + _host + "/ping");
}

QUrl Analytics::paidDownloadStartedEndPoint() const {
    return QUrl("http://" + _host + "/paidDownloadStarted");
}

QUrl Analytics::seedingPaymentsClaimedEndPoint() const {
    return QUrl("http://" + _host + "/seedingPaymentsClaimed");
}

void Analytics::start(quint64 pingInterval) {

    if(_started)
        throw std::runtime_error("Already started");

    // Send start signal to server
    QNetworkReply * reply = _manager->get(QNetworkRequest(startEndPoint()));
    _started = true;

    // Start ping timer
    _pingTimer.setSingleShot(false);
    _pingTimer.setInterval(pingInterval);
    _pingTimer.start();
}

quint64 Analytics::sendAlivePing() {

    // Send ping
    QNetworkReply * reply = _manager->get(QNetworkRequest(pingEndPoint()));

    // Update and return counter
    return _numberOfAlivePings++;
}

void Analytics::addTorrent(const TorrentViewModel * model) {
    QObject::connect(model, &TorrentViewModel::startedBuyerTorrentPlugin, this, &Analytics::buyerTorrentPluginStarted);
    QObject::connect(model, &TorrentViewModel::startedSellerTorrentPlugin, this, &Analytics::sellerTorrentPluginStarted);
}

void Analytics::buyerTorrentPluginStarted(const BuyerTorrentPluginViewModel * model) {
    QObject::connect(model, &BuyerTorrentPluginViewModel::stateChanged, this, &Analytics::buyerTorrentPluginStateChanged);
}

void Analytics::sellerTorrentPluginStarted(const SellerTorrentPluginViewModel * model) {
    QObject::connect(model, &SellerTorrentPluginViewModel::peerAdded, this, &Analytics::sellerPeerAdded);
}

void Analytics::sellerPeerAdded(const SellerPeerPluginViewModel * model) {
    QObject::connect(model, &SellerPeerPluginViewModel::clientStateChanged, this, &Analytics::sellerPeerPluginStateChanged);
}

void Analytics::buyerTorrentPluginStateChanged(BuyerTorrentPlugin::State state) {

    if(state == BuyerTorrentPlugin::State::downloading_pieces) {
        paidDownloadStartedEndPoint();
    }
}

void Analytics::sellerPeerPluginStateChanged(SellerPeerPlugin::ClientState state) {

    if(state == SellerPeerPlugin::ClientState::trying_to_claim_last_payment) {
        claimedSeedingPayment();
    }
}

quint64 Analytics::paidDownloadStarted() {

    // Send download started
    QNetworkReply * reply = _manager->get(QNetworkRequest(paidDownloadStartedEndPoint()));

    // Update and return counter
    return _numberOfPaidDownloadsStarted++;
}

quint64 Analytics::claimedSeedingPayment() {

    // Send
    QNetworkReply * reply = _manager->get(QNetworkRequest(seedingPaymentsClaimedEndPoint()));

    // Update and return counter
    return _numberOfSeedingPaymentsClaimed++;
}

void Analytics::finished(QNetworkReply * reply) {

    if(reply->error() != QNetworkReply::NetworkError::NoError) {
        qDebug() << "QNetworkReply::NetworkError " << reply->errorString();
    } else {
        //qDebug() << "Analytics query " <<  reply->request().url().toString() << " response: " <<reply->readAll();
    }
}
