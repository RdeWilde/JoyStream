/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Saturday 7 November 2015
 */

#ifndef ANALYTICS_HPP
#define ANALYTICS_HPP

#include <Mixpanel/Mixpanel.hpp>
#include <Mixpanel/Event.hpp>

#include <QTimer>
#include <QObject>

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

    static quint64 _defaultPingMsInterval;

    /**
     * These events are currently identical,
     * but they are likeyl to be differentiated later.
     */

    class Start : public Mixpanel::Event {

    public:
        Start(const QString & token, const QString & version);
    };

    class Ping : public Mixpanel::Event {

    public:
        Ping(const QString & token, const QString & version);
    };

    class PaidDownloadStarted : public Mixpanel::Event {

    public:
        PaidDownloadStarted(const QString & token, const QString & version);
    };

    class SeedingPaymentsClaimed : public Mixpanel::Event {

    public:
        SeedingPaymentsClaimed(const QString & token, const QString & version);
    };

    Analytics(QNetworkAccessManager * manager, const QString & MixpanelToken, const QString & applicationVersion);

    // Monitor controller
    void monitor(Controller * controller);

public slots:

    // Send start signal to server
    void start(quint64 pingInterval);

    // Send alive ping to server
    quint64 ping();

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

    // Token used for Mixpanel API
    QString _mixpanelToken;

    // Applicatio versio number
    QString _applicationVersion;

    // Used to regularly fire alive pings
    QTimer _pingTimer;

    // Summary statistics
    bool _started;
    quint64 _numberOfAlivePings;
    quint64 _numberOfPaidDownloadsStarted;
    quint64 _numberOfSeedingPaymentsClaimed;
};

#endif // ANALYTICS_HPP

