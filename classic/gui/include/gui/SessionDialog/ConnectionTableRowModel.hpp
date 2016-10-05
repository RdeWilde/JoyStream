/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 28 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_CONNECTIONTABLEROWMODEL_HPP
#define JOYSTREAM_CLASSIC_GUI_CONNECTIONTABLEROWMODEL_HPP

/// temporary needed for CBStateMachine::InnerStateIndex for now, remove when
/// https://github.com/JoyStream/JoyStream/issues/307 is fixed
#include <core/core.hpp>

class QStandardItem;

class BitcoinDisplaySettings;

namespace joystream {
namespace classic {
namespace gui {

class ConnectionTableRowModel : public QObject {

    Q_OBJECT

public:

    ConnectionTableRowModel(QObject * parent,
                            QStandardItem * hostItem,
                            QStandardItem * stateItem,
                            QStandardItem * fundsItem,
                            QStandardItem * lockItem,
                            QStandardItem * priceItem,
                            QStandardItem * numberOfPaymentsItem,
                            QStandardItem * balanceItem,
                            const BitcoinDisplaySettings * settings);

    int row() const noexcept;

public slots:

    void setHost(const libtorrent::tcp::endpoint &);
    void setState(const core::CBStateMachine::InnerStateIndex &);
    void setFunds(quint64);
    void setRefundLockTime(quint64);
    void setPrice(quint64);
    void setNumberOfPayments(quint64);
    void setBalance(qint64);

private:

    // View model pointers
    // Objects are owned by QStandardItemModel, not us
    QStandardItem * _hostItem,
                  * _stateItem,
                  * _fundsItem,
                  * _lockItem,
                  * _priceItem,
                  * _numberOfPaymentsItem,
                  * _balanceItem;

    // Display settings for bitcoin
    const BitcoinDisplaySettings * _settings;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_CONNECTIONTABLEROWMODEL_HPP
