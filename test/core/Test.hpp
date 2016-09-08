/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2015
 */

#ifndef TEST_HPP
#define TEST_HPP

#include <QtTest/QtTest>

class Test : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase();

    /**
     * Simple integration tests
     * These tests setup a full core node instance, backed
     * by a real libtorrent session, and tries to du a few operations
     */

    void integration_test();

    /**
     * (Private) Alert processing unit tests (core::detail::NodeImpl)
     */

    void listen_succeeded_alert();
    void metadata_received_alert();
    void metadata_failed_alert();
    void add_torrent_alert();
    void torrent_finished_alert();
    void state_update_alert();
    void torrent_removed_alert();
    void torrent_resumed_alert();
    void save_resume_data_alert();
    void save_resume_data_failed_alert();
    void torrent_paused_alert();
    void torrent_checked_alert();
    void peer_connect_alert();
    void peer_disconnected_alert();
    void read_piece_alert();
    void piece_finished_alert();

    // Processing (plugin) alerts
    //void RequestResult(const extension::alert:: * p);
    //void PluginStatus(const extension::alert:: * p);

    /**
     * Public core class unit tests. Primarily we
     * just test that custome type signals work with MOC.
     */

    void payor();
    void payee();
    void cbstatemachine();
    void connection();
    void seller();
    void buying();
    void selling();
    void session();
    void torrent_plugin();
    void peer();
    void torrent();
};

#endif // TEST_HPP
