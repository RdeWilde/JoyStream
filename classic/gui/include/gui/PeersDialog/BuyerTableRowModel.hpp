/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_BUYERTABLEROWMODEL_HPP
#define JOYSTREAM_CLASSIC_GUI_BUYERTABLEROWMODEL_HPP

#include <libtorrent/socket.hpp>

#include <QObject>

class BitcoinDisplaySettings;

class QStandardItem;

namespace joystream {
namespace protocol_wire {
    class BuyerTerms;
}
namespace classic {
namespace gui {

class BuyerTableRowModel : public QObject {

    Q_OBJECT

public:

    BuyerTableRowModel(QStandardItem * hostItem,
                           QStandardItem * maxPriceItem,
                           QStandardItem * maxLockItem,
                           QStandardItem * minNumberOfSellersItem,
                           QStandardItem * maxContractFeePerKbItem,
                           const BitcoinDisplaySettings * settings);

public slots:

    void setHost(const libtorrent::tcp::endpoint & endPoint);

    void setTerms(const protocol_wire::BuyerTerms & terms);

public:
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

#endif // JOYSTREAM_CLASSIC_GUI_BUYERTABLEROWMODEL_HPP
