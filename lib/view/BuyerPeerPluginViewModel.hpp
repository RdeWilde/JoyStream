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

    // Constructor
    BuyerPeerPluginViewModel(BuyerTorrentPluginViewModel * parent, const libtorrent::tcp::endpoint & endPoint);

    // Getters
    libtorrent::tcp::endpoint endPoint() const;
    BuyerPeerPlugin::Status status() const;

public slots:

    // Update status
    void update(const BuyerPeerPlugin::Status & status);

signals:

    // Status signals
    void clientStateChanged(BuyerPeerPlugin::ClientState state);
    void payorSlotChanged(quint32 payorSlot);

private:

    // Peer endpoint
    libtorrent::tcp::endpoint _endPoint;

    // Status of peer plugin
    BuyerPeerPlugin::Status _status;
};

#endif // BUYER_PEER_PLUGIN_VIEW_MODEL_HPP
