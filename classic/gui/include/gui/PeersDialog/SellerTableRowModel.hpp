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

class SellerTableRowModel : public QObject {

    Q_OBJECT

public:

    SellerTableRowModel(QObject * parent,
                        QStandardItem * hostItem,
                        QStandardItem * minPriceItem,
                        QStandardItem * minLockItem,
                        QStandardItem * maxSellersItem,
                        QStandardItem * minContractFeePerKbItem,
                        QStandardItem * settlementFeeItem,
                        const BitcoinDisplaySettings * settings);

public slots:

    void setHost(const libtorrent::tcp::endpoint & endPoint) noexcept;

    void setTerms(const protocol_wire::SellerTerms & terms) noexcept;

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
