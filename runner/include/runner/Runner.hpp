/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 23 2015
 */

#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <core/controller/Controller.hpp> // cant forward declare inner class
#include <libtorrent/torrent_info.hpp> // can this be forward declared perhaps?

namespace Coin {
    enum class Network;
    class Seed;
}

class QNetworkAccessManager;

namespace Runner {

    class ControllerBarrier;

    libtorrent::torrent_info load_torrent(const char * path);

    Controller::Torrent::Configuration create_torrent_configuration(libtorrent::torrent_info & torrentInfo, const QString & savePath);

    // Creates logger, wallet, controller and view
    Controller * create_client(const Controller::Configuration & configuration,
                               bool show_gui,
                               bool use_stdout_logg,
                               const QString & home,
                               const QString & name,
                               const Coin::Seed & seed,
                               Coin::Network network,
                               QNetworkAccessManager * manager,
                               const QString & BlockcypherToken);

    void add_sellers_with_plugin(const Controller::Configuration & configuration,
                                 Runner::ControllerBarrier & barrier,
                                 bool show_gui,
                                 bool use_stdout_logg,
                                 const QString & home,
                                 libtorrent::torrent_info & torrentInfo,
                                 const QVector<SellerTorrentPlugin::Configuration> & configurations,
                                 const QVector<Coin::Seed> seeds,
                                 Coin::Network network,
                                 QNetworkAccessManager * manager,
                                 const QString & BlockcypherToken);

    void add_buyers_with_plugin(const Controller::Configuration & configuration,
                                ControllerBarrier & barrier,
                                bool show_gui,
                                bool use_stdout_logg,
                                const QString & home,
                                libtorrent::torrent_info & torrentInfo,
                                const QVector<BuyerTorrentPlugin::Configuration> & configurations,
                                const QVector<Coin::Seed> & seeds,
                                Coin::Network network,
                                QNetworkAccessManager * manager,
                                const QString & BlockcypherToken);
}

#endif // RUNNER_HPP

