/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 3 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_CONNECTIONTABLEMODEL_HPP
#define JOYSTREAM_CLASSIC_GUI_CONNECTIONTABLEMODEL_HPP

#include <QStandardItemModel>
#include <libtorrent/socket.hpp>

class BitcoinDisplaySettings;

namespace joystream {
namespace classic {
namespace gui {

class ConnectionTableRowModel;

class ConnectionTableModel {

public:

    ConnectionTableModel(BitcoinDisplaySettings * settings);

    ConnectionTableRowModel * add(const libtorrent::tcp::endpoint & endPoint) noexcept;

    libtorrent::tcp::endpoint endPoint(int row) const;

    QAbstractItemModel * model() noexcept;

private:

    QStandardItemModel _model;

    const BitcoinDisplaySettings * _settings;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_CONNECTIONTABLEMODEL_HPP

