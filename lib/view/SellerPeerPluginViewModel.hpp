#ifndef SELLER_PEER_PLUGIN_VIEW_MODEL_HPP
#define SELLER_PEER_PLUGIN_VIEW_MODEL_HPP

//#include "PeerPluginViewModel.hpp"
#include "extension/SellerPeerPlugin.hpp" // SellerPeerPlugin::Status

#include <libtorrent/socket_io.hpp> // libtorrent::tcp::endpoint

class QStandardItem;
class QStandardItemModel;
class SellerTorrentPluginViewModel;
class QDateTime;

class SellerPeerPluginViewModel : public QObject // : public PeerPluginViewModel
{

    Q_OBJECT

public:

    // Constructor from members
    SellerPeerPluginViewModel(SellerTorrentPluginViewModel * parent, const libtorrent::tcp::endpoint & endPoint, QStandardItemModel * model);

    // Update
    void update(const SellerPeerPlugin::Status & status);
    void updateClientState(SellerPeerPlugin::ClientState state);
    void updateContractOutPointItem(const OutPoint & outPoint);
    void updateLockTime(const QDateTime & lockTime);
    void updateFunds(quint64 funds);
    void updatePrice(quint64 price);
    void updateNumberOfPaymentsMadeItem(quint64 numPayments);
    void updateBalanceItem(quint64 balance);

public slots:

signals:

private:

    // Peer endpoint
    libtorrent::tcp::endpoint _endPoint;

    // Model items for seller peer plugin table in SEllerTorrentPluginViewModel
    QStandardItem * _hostItem, // PeerPlugin::endpoint
                  * _clientStateItem, // SellerPeerPlugin::ClientState
                  * _contractOutPointItem,
                  * _fundsItem,
                  * _lockTime,
                  * _priceItem,
                  * _numberOfPaymentsMadeItem,
                  * _balanceItem;
};

#endif // SELLER_PEER_PLUGIN_VIEW_MODEL_HPP
