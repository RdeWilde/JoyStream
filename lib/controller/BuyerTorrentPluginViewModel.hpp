#ifndef BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP
#define BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP

//#include "TorrentPluginViewModel.hpp
#include "extension/BuyerTorrentPlugin.hpp"
#include "PayorViewModel.hpp"

#include <libtorrent/socket_io.hpp>

class BuyerPeerPluginViewModel;

class BuyerTorrentPluginViewModel : public QObject {

    Q_OBJECT

public:

    // Constructor
    BuyerTorrentPluginViewModel(QObject * parent,
                                const BuyerTorrentPlugin::Status & status);

    // Destructor
    //~BuyerTorrentPluginViewModel();

    // Add a model view for a new buyer peer plugin
    void addPeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status);

    // Update
    void update(const BuyerTorrentPlugin::Status & status);

    // Getters
    BuyerTorrentPlugin::State state() const;

    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> buyerPeerPluginViewModels() const;

    const PayorViewModel * payorViewModel() const;

signals:

    // State change
    void stateChanged(BuyerTorrentPlugin::State state);

    // Peer added
    void peerAdded(const BuyerPeerPluginViewModel * model);

private:

    // State of plugin
    BuyerTorrentPlugin::State _state;

    // Maps endpoint to view model for corresponding seller buyer plugin
    // ** We use pointers here since they are QObjects, which cannot be copied **
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> _buyerPeerPluginViewModels;

    // View model of payor
    PayorViewModel _payorViewModel;
};

#endif // BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP
