/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 17 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_PEERTREEVIEWROW_HPP
#define JOYSTREAM_CLASSIC_GUI_PEERTREEVIEWROW_HPP

#include <libtorrent/socket.hpp>

class QStandardItem;

namespace joystream {
namespace extension {
    enum class BEPSupportStatus;
}
namespace classic {
namespace gui {

class PeerTreeViewRow {

public:

    PeerTreeViewRow(QStandardItem * host,
                    QStandardItem * clientName,
                    QStandardItem * BEPSupportStatus);

    QStandardItem * hostItem() const noexcept;
    QStandardItem * clientNameItem() const noexcept;
    QStandardItem * BEPSupportStatusItem() const noexcept;

    void setHost(const libtorrent::tcp::endpoint & endPoint);

    void setClientName(const std::string & clientName);

    void setBEPSupport(const extension::BEPSupportStatus & status);

    int row() const noexcept;

private:

    QStandardItem * _hostItem,
                  * _clientNameItem,
                  * _BEPSupportStatusItem;
};

}
}
}


#endif // JOYSTREAM_CLASSIC_GUI_PEERTREEVIEWROW_HPP

