/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Saturday 7 November 2015
 */

#ifndef ANALYTICS_HPP
#define ANALYTICS_HPP

#include <QTimer>
#include <QObject>

// Default host for analytics calls
#define DEFAULT_ANALYTICS_HOST "analytics.joystream.co"

// Default ping interval (5 min)
#define DEFUALT_PING_MS_INTERVAL 5*(60*1000)

class QNetworkAccessManager;
class Controller;
class TorrentViewModel;
class BuyerTorrentPluginViewModel;
class SellerTorrentPluginViewModel;
class BuyerTorrentPlugin;
class SellerPeerPluginViewModel;
class QNetworkReply;

// Cannot forward declare due to internal class
#include <core/extension/BuyerTorrentPlugin.hpp>

// Cannot forward declare due to internal class
#include <core/extension/SellerPeerPlugin.hpp>

class Analytics : public QObject {

    Q_OBJECT

public:

    static QString _defaultAnalyticsHost;
    static quint64 _defaultPingMsInterval;

    Analytics(QNetworkAccessManager * manager, const QString & host);

    // Monitor controller
    void monitor(Controller * controller);

    // Endpoints
    QUrl startEndPoint() const;
    QUrl pingEndPoint() const;
    QUrl paidDownloadStartedEndPoint() const;
    QUrl seedingPaymentsClaimedEndPoint() const;

public slots:

    // Send start signal to server
    void start(quint64 pingInterval);

    // Send alive ping to server
    quint64 sendAlivePing();

    // Send paid download started to server
    quint64 paidDownloadStarted();

    quint64 claimedSeedingPayment();

    //
    void addTorrent(const TorrentViewModel * model);

    /**
     * Perhaps unbundle the stuff below into analytics
     * classes corresponding to each type?
     */

    // Plugin started
    void buyerTorrentPluginStarted(const BuyerTorrentPluginViewModel * model);
    void sellerTorrentPluginStarted(const SellerTorrentPluginViewModel * model);

    // Seller peer plugin added
    void sellerPeerAdded(const SellerPeerPluginViewModel * model);

    // Plugin state changed
    void buyerTorrentPluginStateChanged(BuyerTorrentPlugin::State state);
    void sellerPeerPluginStateChanged(SellerPeerPlugin::ClientState state);

private slots:

    void finished(QNetworkReply * reply);

private:

    // Network access manager
    QNetworkAccessManager * _manager;

    // Analytics host
    QString _host;

    // Used to regularly fire alive pings
    QTimer _pingTimer;

    // Summary statistics
    bool _started;
    quint64 _numberOfAlivePings;
    quint64 _numberOfPaidDownloadsStarted;
    quint64 _numberOfSeedingPaymentsClaimed;
};

#endif // ANALYTICS_HPP

