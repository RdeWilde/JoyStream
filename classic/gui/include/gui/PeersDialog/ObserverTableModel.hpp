/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 1 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_OBSERVERTABLEMODEL_HPP
#define JOYSTREAM_CLASSIC_GUI_OBSERVERTABLEMODEL_HPP

#include <QStandardItemModel>
#include <libtorrent/socket.hpp>

class BitcoinDisplaySettings;

namespace joystream {
namespace classic {
namespace gui {

class ObserverTableRowModel;

class ObserverTableModel {

public:

    ObserverTableModel(BitcoinDisplaySettings * settings);

    ObserverTableRowModel * add(const libtorrent::tcp::endpoint & endPoint);

    libtorrent::tcp::endpoint endPoint(int row) const;

    QAbstractItemModel * model() noexcept;

private:

    QStandardItemModel _model;

    const BitcoinDisplaySettings * _settings;
};



}
}
}


#endif // JOYSTREAM_CLASSIC_GUI_OBSERVERTABLEMODEL_HPP

