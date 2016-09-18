/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_OBSERVERCONNECTIONTREEVIEWROW_HPP
#define JOYSTREAM_CLASSIC_GUI_OBSERVERCONNECTIONTREEVIEWROW_HPP

#include <libtorrent/socket.hpp>

class BitcoinDisplaySettings;

class QStandardItem;

namespace joystream {
namespace classic {
namespace gui {

class ObserverConnectionTreeViewRow {

public:

    ObserverConnectionTreeViewRow(QStandardItem * hostItem,
                                  const BitcoinDisplaySettings * settings);

    void setHost(const libtorrent::tcp::endpoint & endPoint);

    int row() const noexcept;

private:

    QStandardItem * _hostItem;

    const BitcoinDisplaySettings * _settings;

};

}
}
}


#endif // JOYSTREAM_CLASSIC_GUI_OBSERVERCONNECTIONTREEVIEWROW_HPP
