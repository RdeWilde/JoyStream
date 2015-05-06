#ifndef BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP
#define BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP

//#include "TorrentPluginViewModel.hpp
#include "extension/BuyerPeerPlugin.hpp"
#include "extension/BuyerTorrentPlugin.hpp"

#include <libtorrent/socket_io.hpp>

#include <QStandardItemModel>

class BuyerPeerPluginViewModel;
class TorrentViewModel;

class BuyerTorrentPluginViewModel : public QObject { // : public TorrentPluginViewModel {

    Q_OBJECT

public:

    // Default constructor
    BuyerTorrentPluginViewModel();

    // Constructor from members
    BuyerTorrentPluginViewModel(TorrentViewModel * parent, const libtorrent::sha1_hash & infoHash);

    // Update status
    void update(const BuyerTorrentPlugin::Status & status);
    void updatePeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status);

    // Getters and setters
    QStandardItemModel * buyerPeerPluginTableViewModel();

private:

    // Info hash of corresponding torrent
    libtorrent::sha1_hash _infoHash;

    // Buyer peer plugin table view model
    QStandardItemModel _buyerPeerPluginTableViewModel;

    // Maps endpoint to view model for corresponding seller buyer plugin
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> _buyerPeerPluginViewModels;
};

#endif // BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP
