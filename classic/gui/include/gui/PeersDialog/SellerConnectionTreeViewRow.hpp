/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_SELLERCONNECTIONTREEVIEWROW_HPP
#define JOYSTREAM_CLASSIC_GUI_SELLERCONNECTIONTREEVIEWROW_HPP

#include <libtorrent/socket.hpp>

#include <QObject>

class BitcoinDisplaySettings;

class QStandardItem;

namespace joystream {
namespace protocol_wire {
    class SellerTerms;
}
namespace classic {
namespace gui {

class SellerConnectionTreeViewRow {

public:

    SellerConnectionTreeViewRow(QStandardItem * hostItem,
                                QStandardItem * minPriceItem,
                                QStandardItem * minLockItem,
                                QStandardItem * maxSellersItem,
                                QStandardItem * minContractFeePerKbItem,
                                QStandardItem * settlementFeeItem,
                                const BitcoinDisplaySettings * settings);

    QStandardItem * hostItem() const noexcept;
    QStandardItem * minPriceItem() const noexcept;
    QStandardItem * minLockItem() const noexcept;
    QStandardItem * maxSellersItem() const noexcept;
    QStandardItem * minContractFeePerKbItem() const noexcept;
    QStandardItem * settlementFeeItem() const noexcept;

    void setHost(const libtorrent::tcp::endpoint & endPoint);

    void setTerms(const protocol_wire::SellerTerms & terms);

    int row() const noexcept;

private:

    QStandardItem * _hostItem,
                  * _minPriceItem,
                  * _minLockItem,
                  * _maxSellersItem,
                  * _minContractFeePerKbItem,
                  * _settlementFeeItem;

    const BitcoinDisplaySettings * _settings;
};


}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_SELLERCONNECTIONTREEVIEWROW_HPP
