#ifndef BUYER_PEER_PLUGIN_VIEW_MODEL_HPP
#define BUYER_PEER_PLUGIN_VIEW_MODEL_HPP

//#include "PeerPluginViewModel.hpp"
#include "extension/BuyerPeerPlugin.hpp" // BuyerPeerPlugin::Status
#include "extension/PaymentChannel/Payor.hpp"

#include <libtorrent/socket_io.hpp> // libtorrent::tcp::endpoint

class QStandardItem;
class QStandardItemModel;
class BuyerTorrentPluginViewModel;
class QDateTime;

class BuyerPeerPluginViewModel : public QObject // : public PeerPluginViewModel
{
    Q_OBJECT

public:

    // Constructor from members
    BuyerPeerPluginViewModel(BuyerTorrentPluginViewModel * parent, const libtorrent::tcp::endpoint & endPoint, QStandardItemModel * model);

    // Update
    void update(const BuyerPeerPlugin::Status & peerStatus, const Payor::Channel::Status & channelStatus);

    void updateState(BuyerPeerPlugin::ClientState state);
    void updatePayorSlotItem(quint32 payorSlot);
    void updateLockTime(const QDateTime & lockTime);
    void updateFunds(quint64 funds);
    void updatePrice(quint64 price);
    void updateNumberOfPaymentsMadeItem(quint64 numPayments);
    void updateBalanceItem(quint64 balance);

public slots:

signals:

private:

    // Peer endpoint
    // WHY DO WE NEED THIS?
    libtorrent::tcp::endpoint _endPoint;

    // Regular torrent level information

    // Model items for buyer peer plugin table in BuyerTorrentPluginViewModel
    QStandardItem * _hostItem, // PeerPlugin::endpoint
                  * _stateItem; // BuyerPeerPlugin::ClientState

                  * _payorSlotItem, // BuyerPeerPlugin::_payorSlot
                  * _fundsItem, //
                  * _lockTime, // Channel::Status::inde
                  * _priceItem, // Channel::Status::_price
                  * _numberOfPaymentsMadeItem, // Channel::Status::inde
                  * _balanceItem; // deduced


};

#endif // BUYER_PEER_PLUGIN_VIEW_MODEL_HPP
