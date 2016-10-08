/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 1 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_BUYERTABLEMODEL_HPP
#define JOYSTREAM_CLASSIC_GUI_BUYERTABLEMODEL_HPP

#include <QStandardItemModel>
#include <libtorrent/socket.hpp>

class BitcoinDisplaySettings;

namespace joystream {
namespace classic {
namespace gui {

class BuyerTableRowModel;

class BuyerTableModel {

public:

    BuyerTableModel(BitcoinDisplaySettings * settings);

    BuyerTableRowModel * add(const libtorrent::tcp::endpoint & endPoint);

    /**
     * @brief Remove row with given index from model
     * @param row index of row to be removed
     * @throws std::runtime_error if index is invalid
     */
    void remove(int row);

    libtorrent::tcp::endpoint endPoint(int row) const;

    QAbstractItemModel * model() noexcept;

private:

    QStandardItemModel _model;

    const BitcoinDisplaySettings * _settings;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_BUYERTABLEMODEL_HPP
