/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 1 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_PEERTABLEMODEL_HPP
#define JOYSTREAM_CLASSIC_GUI_PEERTABLEMODEL_HPP

#include <QStandardItemModel>
#include <libtorrent/socket.hpp>

class BitcoinDisplaySettings;

namespace joystream {
namespace classic {
namespace gui {

class PeerTableRowModel;

class PeerTableModel {

public:

    PeerTableModel(BitcoinDisplaySettings * settings);

    ~PeerTableModel();

    PeerTableRowModel * add(const libtorrent::tcp::endpoint & endPoint);

    void remove(const libtorrent::tcp::endpoint & endPoint);

    std::map<libtorrent::tcp::endpoint, PeerTableRowModel *> rowModels() const noexcept;

    QStandardItemModel * standardItemModel() noexcept;

private:

    std::map<libtorrent::tcp::endpoint, PeerTableRowModel *> _rowModels;

    QStandardItemModel _standardItemModel;

    const BitcoinDisplaySettings * _settings;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_PEERTABLEMODEL_HPPs

