/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#include <gui/PeersDialog/ObserverTableRowModel.hpp>
#include <gui/Common.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

ObserverTableRowModel::ObserverTableRowModel(QStandardItem * hostItem,
                                                             const BitcoinDisplaySettings * settings)
    : _hostItem(hostItem)
    , _settings(settings){
}

void ObserverTableRowModel::setHost(const libtorrent::tcp::endpoint & endPoint) {
    _hostItem->setText(Common::toString(endPoint));
}

int ObserverTableRowModel::row() const noexcept {
    return _hostItem->row();
}

}
}
}
