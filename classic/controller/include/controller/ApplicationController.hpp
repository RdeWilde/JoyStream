/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_APPLICATIONCONTROLLER_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_APPLICATIONCONTROLLER_HPP

#include <QStandardItemModel>

#include <gui/gui.hpp>

#include <libtorrent/sha1_hash.hpp>

#include <map>
#include <memory>

namespace joystream {
namespace core {
    class Node;
    class Torrent;
}
namespace classic {
namespace gui {
    class MainWindow;
}
namespace controller {

class Torrent;

class ApplicationController {

public:

    static void setTorrentTableModelHorizontalHeaderLabels(QStandardItemModel * model) noexcept;

    ApplicationController();

    ~ApplicationController();

    void addTorrent(core::Torrent * torrent);

    void removeTorrent(const libtorrent::tcp::endpoint & endPoint);

signals:

private:

    // Primary app kit
    // Appkit _appkit

    core::Node _node;

    //bitcoin::SPVWallet _wallet;

    // Main window
    gui::MainWindow _mainWindow;

    // TrayMenuManager _trayMenyManager

    // Model for torrent table in
    //QStandardItemModel _mainWindowTorrentTableModel;

    gui::TorrentTableModel _mainWindowTorrentTableModel;

    // Mapping of endpoint to torrent
    std::map<libtorrent::sha1_hash, std::unique_ptr<Torrent>> _torrents;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_APPLICATIONCONTROLLER_HPP
