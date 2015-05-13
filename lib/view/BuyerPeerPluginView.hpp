#ifndef BUYER_PEER_PLUGIN_VIEW_HPP
#define BUYER_PEER_PLUGIN_VIEW_HPP

#include "extension/BuyerPeerPlugin.hpp"

#include <QObject>
#include <QMenu>
#include <QAction>

class QStandardItem;
class QStandardItemModel;

class BuyerPeerPluginView : public QObject
{
    Q_OBJECT
public:
    explicit BuyerPeerPluginView(const libtorrent::tcp::endpoint & endPoint,
                                 BuyerPeerPlugin::ClientState state,
                                 quint32 payorSlot,
                                 QStandardItemModel * model);

    // Text conversion routines
    static QString endPointToString(const libtorrent::tcp::endpoint & endPoint);
    static QString clientStateToString(BuyerPeerPlugin::ClientState state);
    static QString payorSlotToString(quint32 payorSlot);

public slots:

    // Update fields
    void updateEndPoint(const libtorrent::tcp::endpoint & endPoint);
    void updateClientState(BuyerPeerPlugin::ClientState clientState);
    void updatePayorSlot(quint32 payorSlot);

signals:

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
