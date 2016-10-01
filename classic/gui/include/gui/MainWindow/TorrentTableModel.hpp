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

    ~TorrentTableModel();

    TorrentTableRowModel * add(const libtorrent::sha1_hash & infoHash);

    void remove(const libtorrent::sha1_hash & infoHash);

    std::map<libtorrent::sha1_hash, TorrentTableRowModel *> rowModels() const noexcept;

    QStandardItemModel * standardItemModel() noexcept;

private:

    std::map<libtorrent::sha1_hash, TorrentTableRowModel *> _rowModels;

    QStandardItemModel _standardItemModel;

    const BitcoinDisplaySettings * _settings;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_TORRENTTABLEMODEL_HPP
