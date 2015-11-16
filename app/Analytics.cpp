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


// Default ping interval (5 min)
#define DEFUALT_PING_MS_INTERVAL 5*(60*1000)

quint64 Analytics::_defaultPingMsInterval = DEFUALT_PING_MS_INTERVAL;

static QString _MixpanelToken;

Analytics::Start::Start(const QString & token, const QString & version)
    : Mixpanel::Event("start", Event::Properties(token)) {

    // Save version
    _properties["version"] = version;
}

Analytics::Ping::Ping(const QString & token, const QString & version)
    : Mixpanel::Event("ping", Event::Properties(token)) {

    // Save version
    _properties["version"] = version;
}

Analytics::PaidDownloadStarted::PaidDownloadStarted(const QString & token, const QString & version)
    : Mixpanel::Event("paid_download_started", Event::Properties(token)) {

    // Save version
    _properties["version"] = version;
}

Analytics::SeedingPaymentsClaimed::SeedingPaymentsClaimed(const QString & token, const QString & version)
    : Mixpanel::Event("seeding_payments_claimed", Event::Properties(token)) {

    // Save version
    _properties["version"] = version;
}

Analytics::Analytics(QNetworkAccessManager * manager, const QString & MixpanelToken, const QString & applicationVersion)
    : _manager(manager)
    , _mixpanelToken(MixpanelToken)
    , _applicationVersion(applicationVersion)
    , _started(false)
    , _numberOfAlivePings(0)
    , _numberOfPaidDownloadsStarted(0) {

    // Capture timer events
    QObject::connect(&_pingTimer, &QTimer::timeout, this, &Analytics::ping);
}

void Analytics::monitor(Controller * controller) {

    QObject::connect(controller, &Controller::addedTorrent, this, &Analytics::paidDownloadStarted);
}

void Analytics::start(quint64 pingInterval) {

    if(_started)
        throw std::runtime_error("Already started");

    // Send start event
    Start start(_mixpanelToken, _applicationVersion);
    Mixpanel::sendEvent(start);

    // Start ping timer
    _pingTimer.setSingleShot(false);
    _pingTimer.setInterval(pingInterval);
    _pingTimer.start();
}

quint64 Analytics::ping() {

    // Send ping
    Ping ping(_mixpanelToken, _applicationVersion);
    Mixpanel::sendEvent(ping);

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
        paidDownloadStarted();
    }
}

void Analytics::sellerPeerPluginStateChanged(SellerPeerPlugin::ClientState state) {

    if(state == SellerPeerPlugin::ClientState::trying_to_claim_last_payment) {
        claimedSeedingPayment();
    }
}

quint64 Analytics::paidDownloadStarted() {

    // Send PaidDownloadStarted
    PaidDownloadStarted paidDownloadStarted(_mixpanelToken, _applicationVersion);
    Mixpanel::sendEvent(paidDownloadStarted);

    // Update and return counter
    return _numberOfPaidDownloadsStarted++;
}

quint64 Analytics::claimedSeedingPayment() {

    // Send SeedingPaymentsClaimed
    SeedingPaymentsClaimed claimedSeedingPayment(_mixpanelToken, _applicationVersion);
    Mixpanel::sendEvent(claimedSeedingPayment);

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
