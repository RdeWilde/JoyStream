#ifndef SELLER_PEER_PLUGIN_VIEW_MODEL_HPP
#define SELLER_PEER_PLUGIN_VIEW_MODEL_HPP

//#include "PeerPluginViewModel.hpp"
#include "extension/SellerPeerPlugin.hpp" // SellerPeerPlugin::Status
#include "PayeeViewModel.hpp"

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
    SellerPeerPluginViewModel(QObject * parent,
                              const libtorrent::tcp::endpoint & endPoint,
                              const SellerPeerPlugin::Status & status);

    // Update
    void update(const SellerPeerPlugin::Status & status);

    // Getters
    libtorrent::tcp::endpoint endPoint() const;
    SellerPeerPlugin::ClientState clientState() const;
    const PayeeViewModel * payeeViewModel() const;
    //QList<int> fullPiecesSent() const;

signals:

    // Update
    void clientStateChanged(SellerPeerPlugin::ClientState state);

private:

    // Peer endpoint
    libtorrent::tcp::endpoint _endPoint;

    // State of client
    SellerPeerPlugin::ClientState _clientState;

    // Payee view model
    PayeeViewModel _payeeViewModel;

    // Full pieces sent
    //QList<int> _fullPiecesSent;
};

#endif // SELLER_PEER_PLUGIN_VIEW_MODEL_HPP
