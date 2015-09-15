/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 30 2015
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

class Test : public QObject
{
    Q_OBJECT

private slots:

    /**
     * Test routines
     */

    void basic();

    void download_and_streaming();

    void paid_uploading();

private:

    libtorrent::torrent_info load_torrent(const char * path);

    Controller::Torrent::Configuration create_torrent_configuration(libtorrent::torrent_info & torrentInfo, const QString & name);

    Controller * create_controller(Controller::Configuration controllerConfiguration,
                                   QNetworkAccessManager * manager,
                                   bool show_gui,
                                   bool use_stdout_logg,
                                   libtorrent::torrent_info & torrentInfo,
                                   const QString & name);

    void add_buyers_with_plugin(Controller::Configuration controllerConfiguration,
                                QNetworkAccessManager * manager,
                                ControllerTracker & controllerTracker,
                                bool show_gui,
                                bool use_stdout_logg,
                                libtorrent::torrent_info & torrentInfo,
                                const QVector<BuyerTorrentPlugin::Configuration> & configurations);

    void add_sellers_with_plugin(Controller::Configuration controllerConfiguration,
                                 QNetworkAccessManager * manager,
                                 ControllerTracker & controllerTracker,
                                 bool show_gui,
                                 bool use_stdout_logg,
                                 libtorrent::torrent_info & torrentInfo,
                                 const QVector<SellerTorrentPlugin::Configuration> & configurations);

};

#endif // TEST_HPP
