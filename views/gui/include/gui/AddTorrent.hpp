/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef ADD_TORRENT_HPP
#define ADD_TORRENT_HPP

#include <QDialog>
#include <QString>

#include <core/controller/Controller.hpp>

class AddTorrent
{

public:

    // Constructors
    AddTorrent(Controller * controller, const QString & resource, bool isTorrentFile, const std::string save_path);

    ~AddTorrent();

private:

    // Controller
    Controller * _controller;

    /**
     * INPUTS CONTROLLER VIEW
     */

    // Torrent resouce, i.e. magnet link or file location
    QString _resource;

    // Indicates whether adding was magnet link or file location
    bool _isTorrentFile;

    /**
     * PARSED STATE
     */

    // TORRENT FILE CASE ===================

    // Torrent information : is pointer since there is only one constructor, and in this constructor we need a test which
    // does not allow this field to be set in initialization list, this in combination with the fact that torrent_info has
    // no assignment operator forces us to save as pointer member.
    libtorrent::torrent_info * _torrentInfo;

    // MAGNET LINK CASE ===================

    // Magnet link url
    std::string _url;

    // parse_magnet_uri
    libtorrent::add_torrent_params _params;
};

#endif // ADD_TORRENT_HPP
