/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#include <gui/PeersDialog/ObserverConnectionTreeViewRow.hpp>
#include <gui/Common.hpp>
#include <common/BitcoinDisplaySettings.hpp>

#include <QStandardItem>

namespace joystream {
namespace classic {
namespace gui {

ObserverConnectionTreeViewRow::ObserverConnectionTreeViewRow(QStandardItem * hostItem,
                                                             const BitcoinDisplaySettings * settings)
    : _hostItem(hostItem)
    , _settings(settings){
}

void ObserverConnectionTreeViewRow::setHost(const libtorrent::tcp::endpoint & endPoint) {
    _hostItem->setText(Common::toString(endPoint));
}

int ObserverConnectionTreeViewRow::row() const noexcept {
    return _hostItem->row();
}

}
}
}
