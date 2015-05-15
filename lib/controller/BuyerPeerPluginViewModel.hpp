#ifndef BUYER_PEER_PLUGIN_VIEW_MODEL_HPP
#define BUYER_PEER_PLUGIN_VIEW_MODEL_HPP

//#include "PeerPluginViewModel.hpp"
#include "extension/BuyerPeerPlugin.hpp" // BuyerPeerPlugin::Status

class BuyerPeerPluginViewModel : public QObject
{
    Q_OBJECT

public:

    // Constructor
    BuyerPeerPluginViewModel(QObject * parent,
                             const libtorrent::tcp::endpoint & endPoint,
                             const BuyerPeerPlugin::Status & status);

    // Update status
    void update(const BuyerPeerPlugin::Status & status);

    // Getters
    libtorrent::tcp::endpoint endPoint() const;
    BuyerPeerPlugin::Status status() const;

signals:

    // Status change
    void clientStateChanged(BuyerPeerPlugin::ClientState state);
    void payorSlotChanged(quint32 payorSlot);

private:

    // Peer endpoint
    libtorrent::tcp::endpoint _endPoint;

    // Status of peer plugin
    BuyerPeerPlugin::Status _status;
};

#endif // BUYER_PEER_PLUGIN_VIEW_MODEL_HPP
