#ifndef SELLER_PEER_PLUGIN_VIEW_HPP
#define SELLER_PEER_PLUGIN_VIEW_HPP

#include "extension/SellerPeerPlugin.hpp"

#include <QObject>
#include <QMenu>
#include <QAction>

class QStandardItem;
class QStandardItemModel;
class SellerPeerPluginViewModel;

class SellerPeerPluginView : public QObject
{
    Q_OBJECT
public:

    // Constructor
    SellerPeerPluginView(QObject *parent,
                         const SellerPeerPluginViewModel * peerModel,
                         QStandardItemModel * itemModel);

    // Destructor
    ~SellerPeerPluginView();

    // Text conversion routines
    static QString endPointToString(const libtorrent::tcp::endpoint & endPoint);
    static QString clientStateToString(SellerPeerPlugin::ClientState state);
    static QString contractOutPointString(const OutPoint & o);
    static QString fundsToString(quint64 funds);
    static QString refundLockTimeString(quint32 refundLockTime);
    static QString priceToString(quint64 price);
    static QString numberOfPaymentMadeToString(quint32 numberOfPaymentMade);
    static QString balanceToString(quint32 balance);

signals:

public slots:

    // Update fields
    void updateEndPoint(const libtorrent::tcp::endpoint & endPoint);
    void updateClientState(SellerPeerPlugin::ClientState state);


    void updateContractOutPointItem(const OutPoint & o);
    void updateFunds(quint64 funds);
    void updateRefundLockTime(quint32 refundLockTime);
    void updatePrice(quint64 price);
    void updateNumberOfPaymentMade(quint64 numberOfPaymentMade);
    void updateBalance(quint32 balance);

private:

    // Reference to model which takes ownership of items
    QStandardItemModel * _itemModel;

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
