/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_BUYERCONNECTIONTREEVIEWROW_HPP
#define JOYSTREAM_CLASSIC_GUI_BUYERCONNECTIONTREEVIEWROW_HPP

#include <libtorrent/socket.hpp>

class BitcoinDisplaySettings;

class QStandardItem;

namespace joystream {
namespace protocol_wire {
    class BuyerTerms;
}
namespace classic {
namespace gui {

class BuyerConnectionTreeViewRow {

public:

    BuyerConnectionTreeViewRow(QStandardItem * hostItem,
                               QStandardItem * maxPriceItem,
                               QStandardItem * maxLockItem,
                               QStandardItem * minNumberOfSellersItem,
                               QStandardItem * maxContractFeePerKbItem,
                               const BitcoinDisplaySettings * settings);

    QStandardItem * hostItem() const noexcept;
    QStandardItem * maxPriceItem() const noexcept;
    QStandardItem * maxLockItem() const noexcept;
    QStandardItem * minNumberOfSellersItem() const noexcept;
    QStandardItem * maxContractFeePerKbItem() const noexcept;

    void setHost(const libtorrent::tcp::endpoint & endPoint);

    void setTerms(const protocol_wire::BuyerTerms & terms);

    int row() const noexcept;

private:

    QStandardItem * _hostItem,
                  * _maxPriceItem,
                  * _maxLockItem,
                  * _minNumberOfSellersItem,
                  * _maxContractFeePerKbItem;

    const BitcoinDisplaySettings * _settings;
};



}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_BUYERCONNECTIONTREEVIEWROW_HPP
