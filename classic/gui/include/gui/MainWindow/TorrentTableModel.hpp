/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 28 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_TORRENTTABLEMODEL_HPP
#define JOYSTREAM_CLASSIC_GUI_TORRENTTABLEMODEL_HPP

#include <QStandardItemModel>
#include <libtorrent/sha1_hash.hpp>

class BitcoinDisplaySettings;

namespace joystream {
namespace classic {
namespace gui {

class TorrentTableRowModel;

class TorrentTableModel {

public:

    TorrentTableModel(BitcoinDisplaySettings * settings);

    /**
     * @brief Add row, for torrent with given info_hash, to model, and return model
     * @param infoHash info hash of torrent for which a row should be added
     * @return model of row for torrent, which has underlying itemmodel as parent, and
     * Qt::UserRole set to this infoHash
     */
    TorrentTableRowModel * add(const libtorrent::sha1_hash & infoHash) noexcept;

    /**
     * @brief Recover info has of torrrent which corresponds to row with given index
     * @param row index of row
     * @return info hash of torrent
     * @throws if now such row exists
     */
    libtorrent::sha1_hash infoHash(int row) const;

    /**
     * @brief Model for table
     * @return model
     */
    QAbstractItemModel * model() noexcept;

private:

    QStandardItemModel _model;

    const BitcoinDisplaySettings * _settings;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_TORRENTTABLEMODEL_HPP
