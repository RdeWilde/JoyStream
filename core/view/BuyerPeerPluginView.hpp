/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef BUYER_PEER_PLUGIN_VIEW_HPP
#define BUYER_PEER_PLUGIN_VIEW_HPP

#include "extension/BuyerPeerPlugin.hpp"

#include <QObject>
#include <QMenu>
#include <QAction>

class QStandardItem;
class BuyerPeerPluginViewModel;

class BuyerPeerPluginView : public QObject
{
    Q_OBJECT
public:

    // Constructor
    BuyerPeerPluginView(QObject * parent,
                        const BuyerPeerPluginViewModel * peerModel,
                        QStandardItem * endPointItem,
                        QStandardItem * clientStateItem,
                        QStandardItem * payorSlotItem);

    // Text conversion routines
    static QString endPointToString(const libtorrent::tcp::endpoint & endPoint);
    static QString clientStateToString(BuyerPeerPlugin::ClientState state);
    static QString payorSlotToString(quint32 payorSlot);

    // Getters and setters
    QStandardItem *endPointItem() const;
    void setEndPointItem(QStandardItem *endPointItem);

    QStandardItem *clientStateItem() const;
    void setClientStateItem(QStandardItem *clientStateItem);

    QStandardItem *payorSlotItem() const;
    void setPayorSlotItem(QStandardItem *payorSlotItem);

signals:

public slots:

    // Update fields
    void updateEndPoint(const libtorrent::tcp::endpoint & endPoint);
    void updateClientState(BuyerPeerPlugin::ClientState clientState);
    void updatePayorSlot(quint32 payorSlot);

private:

    // View model pointers
    // Objects are owned by QStandardItemModel passed to ctr
    QStandardItem * _endPointItem,
                  * _clientStateItem,
                  * _payorSlotItem;

    // Context menu
    //QMenu _torrentTableContextMenu;

    // Context menu actions
    //QAction _pause;
};

#endif // BUYER_PEER_PLUGIN_VIEW_HPP
