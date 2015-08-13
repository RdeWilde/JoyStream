/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef CHANNEL_VIEW_HPP
#define CHANNEL_VIEW_HPP

#include <core/extension/PaymentChannel/Payor.hpp>

#include <QObject>
#include <QMenu>
#include <QAction>

class QStandardItem;
class QStandardItemModel;
class ChannelViewModel;
class BitcoinDisplaySettings;

class ChannelView : public QObject
{
    Q_OBJECT
public:

    // Constructor
    ChannelView(QObject * parent,
                const ChannelViewModel * channelViewModel,
                QStandardItemModel * model,
                const BitcoinDisplaySettings * settings);

    // Text conversion routines
    static QString stateToString(Payor::Channel::State state);
    static QString fundsToString(quint64 funds, const BitcoinDisplaySettings * settings);
    static QString refundLockTimeToString(quint32 refundLockTime);
    static QString priceToString(quint64 price, const BitcoinDisplaySettings * settings);
    static QString numberOfPaymentsMadeToString(quint64 numberOfPaymentsMade);
    static QString balanceToString(quint64 balance, const BitcoinDisplaySettings * settings);

public slots:

    // Update channel status fields
    void updateState(Payor::Channel::State state);
    void updateFunds(quint64 funds);
    void updateRefundLockTime(quint32 refundLockTime);
    void updatePrice(quint64 price);
    void updateNumberOfPaymentsMade(quint64 numberOfPaymentsMade);
    void updateBalance(quint64 balance);

signals:

private:

    // BitCoin display settings
    const BitcoinDisplaySettings * _settings;

    // Index of channel in payor
    // Is required to send signals based on user interaction
    quint32 _index;

    // View model pointers
    // Objects are owned by QStandardItemModel passed to ctr
    QStandardItem * _indexItem,
                  * _stateItem,
                  * _fundsItem,
                  * _refundLockTimeItem,
                  * _priceItem,
                  * _numberOfPaymentMadeItem,
                  * _balanceItem;

    // Context menu
    //QMenu _torrentTableContextMenu;

    // Context menu actions
    //QAction _pause;
};

#endif // CHANNEL_VIEW_HPP
