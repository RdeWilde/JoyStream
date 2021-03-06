/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SELLER_PEER_PLUGIN_VIEW_HPP
#define SELLER_PEER_PLUGIN_VIEW_HPP

#include <core/extension/SellerPeerPlugin.hpp>

#include <QObject>
#include <QMenu>
#include <QAction>

class QStandardItem;
class SellerPeerPluginViewModel;
class BitcoinDisplaySettings;

class SellerPeerPluginView : public QObject
{
    Q_OBJECT
public:

    // Constructor
    SellerPeerPluginView(QObject *parent,
                         const SellerPeerPluginViewModel * peerModel,
                         const BitcoinDisplaySettings * settings,
                         QStandardItem * endPointItem,
                         QStandardItem * clientStateItem,
                         QStandardItem * contractOutPointItem,
                         QStandardItem * fundsItem,
                         QStandardItem * refundLockTimeItem,
                         QStandardItem * priceItem,
                         QStandardItem * numberOfPaymentMadeItem,
                         QStandardItem * balanceItem);

    // Text conversion routines
    static QString endPointToString(const libtorrent::tcp::endpoint & endPoint);
    static QString clientStateToString(SellerPeerPlugin::ClientState state);
    static QString contractOutPointString(const Coin::typesafeOutPoint & o);
    QString fundsToString(quint64 funds);
    static QString refundLockTimeString(quint32 refundLockTime);
    QString priceToString(quint64 price);
    static QString numberOfPaymentMadeToString(quint32 numberOfPaymentMade);
    QString balanceToString(quint32 balance);

    // Getters and setters
    QStandardItem *endPointItem() const;
    void setEndPointItem(QStandardItem *endPointItem);

    QStandardItem *clientStateItem() const;
    void setClientStateItem(QStandardItem *clientStateItem);

    QStandardItem *contractOutPointItem() const;
    void setContractOutPointItem(QStandardItem *contractOutPointItem);

    QStandardItem *fundsItem() const;
    void setFundsItem(QStandardItem *fundsItem);

    QStandardItem *refundLockTimeItem() const;
    void setRefundLockTimeItem(QStandardItem *refundLockTimeItem);

    QStandardItem *priceItem() const;
    void setPriceItem(QStandardItem *priceItem);

    QStandardItem *numberOfPaymentMadeItem() const;
    void setNumberOfPaymentMadeItem(QStandardItem *numberOfPaymentMadeItem);

    QStandardItem *balanceItem() const;
    void setBalanceItem(QStandardItem *balanceItem);

signals:

public slots:

    // Update fields
    void updateEndPoint(const libtorrent::tcp::endpoint & endPoint);
    void updateClientState(SellerPeerPlugin::ClientState state);


    void updateContractOutPointItem(const Coin::typesafeOutPoint & o);
    void updateFunds(quint64 funds);
    void updateRefundLockTime(quint32 refundLockTime);
    void updatePrice(quint64 price);
    void updateNumberOfPaymentMade(quint64 numberOfPaymentMade);
    void updateBalance(quint64 balance);

private:

    // Display settings for bitcoin
    const BitcoinDisplaySettings * _settings;

    // View model pointers
    // Objects are owned by QStandardItemModel passed to ctr
    QStandardItem * _endPointItem,
                  * _clientStateItem,
                  * _contractOutPointItem,
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

#endif // SELLER_PEER_PLUGIN_VIEW_HPP
