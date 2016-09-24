/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_APPLICATIONCONTROLLER_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_APPLICATIONCONTROLLER_HPP

#include <libtorrent/socket.hpp>

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

    ApplicationController();

    ~ApplicationController();

    /// Events from core


    void addTorrent(core::Torrent * torrent);

    //
    void removeTorrent(const libtorrent::tcp::endpoint & endPoint);

    /// Events from application


private:

    // appkit (core+wallet)

    // main window?
    //gui::MainWindow * _mainWindow;

    // tray manager?



    // Mapping of endpoint to torrent
    std::map<libtorrent::tcp::endpoint, std::unique_ptr<Torrent>> _torrents;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_APPLICATIONCONTROLLER_HPP
