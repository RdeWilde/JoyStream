/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/AddTorrent.hpp>

#include <libtorrent/magnet_uri.hpp> // libtorrent::parse_magnet_uri
#include <libtorrent/add_torrent_params.hpp> // parsing torrent magnet/file

#include <QDebug>
#include <QStandardPaths>

AddTorrent::AddTorrent(Controller * controller, const QString & resource, bool isTorrentFile, std::string save_path)
    : _controller(controller)
    , _resource(resource)
    , _isTorrentFile(isTorrentFile)
    , _torrentInfo(NULL) {

    // Error code
    libtorrent::error_code ec;

    if(_isTorrentFile) { // From torrent file

        // Load torrent file
        _torrentInfo = new libtorrent::torrent_info(_resource.toStdString().c_str(), ec);

        // Was torrent file valid?
        if(ec) {
            qDebug() << "Invalid torrent file: " << ec.message().c_str();
            return;
        }

    } else { // From magnet link

        // Get magnet link
        _url = _resource.toStdString();

        // Parse link to get info_hash
        libtorrent::parse_magnet_uri(_url, _params, ec);

        // Was magnet link malformed
        if(ec) {
            qDebug() << "Invalid magnet link: " << ec.message().c_str();
            return;
        }
    }

    // Info hash
    libtorrent::sha1_hash info_hash;

    if(_isTorrentFile)
        info_hash = _torrentInfo->info_hash();
    else
        info_hash = _params.info_hash;

    // Name
    std::string name;

    if(_isTorrentFile)
        name = _torrentInfo->name();
    else
        name = _params.name;

    // Resume data
    std::vector<char> resume_data;

    // Create configuration for adding torrent
    libtorrent::torrent_info * t;

    // make copy
    if(_torrentInfo != NULL)
        t = new libtorrent::torrent_info(*_torrentInfo);

    Controller::Torrent::Configuration configuration(info_hash,
                                                      name,
                                                      save_path,
                                                      resume_data,
                                                      libtorrent::add_torrent_params::flag_update_subscribe,
                                                      t);

    // Add torrent, user will later have to supply torrent plugin configuration
    _controller->addTorrent(configuration);
}

AddTorrent::~AddTorrent() {

    // Delete torrent information we allocated in constructor
    if(_isTorrentFile)
        delete _torrentInfo;
}

