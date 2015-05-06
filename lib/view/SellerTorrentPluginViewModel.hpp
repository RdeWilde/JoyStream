#ifndef SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP
#define SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP

//#include "TorrentPluginViewModel.hpp"
//#include "extension/SellerTorrentPlugin.hpp" //SellerTorrentPlugin::Configuration
#include "extension/SellerTorrentPlugin.hpp"
#include "extension/SellerPeerPlugin.hpp"

#include <QMap>
#include <QStandardItemModel>

#include <libtorrent/socket_io.hpp>

class SellerPeerPluginViewModel;
class TorrentViewModel;

class SellerTorrentPluginViewModel : public QObject // : public TorrentPluginViewModel
{
    Q_OBJECT

public:

    static const char * columnTitles[];
    static const int numberOfColumns;

    // Default constructor
    //SellerTorrentPluginViewModel(TorrentViewModel * parent);

    // Constructor from members
    SellerTorrentPluginViewModel(TorrentViewModel * parent, const libtorrent::sha1_hash & infoHash);

    // Add a model view for a new seller peer plugin
    void addPeer(const libtorrent::tcp::endpoint & endPoint); //, const SellerTorrentPlugin::Configuration & configuration);

    // Update status
    void update(const SellerTorrentPlugin::Status & status);
    void updatePeer(const libtorrent::tcp::endpoint & endPoint, const SellerPeerPlugin::Status & status);

    // Getters and setters
    QStandardItemModel * sellerPeerPluginTableViewModel();

public slots:

signals:

private:

    // Info hash of corresponding torrent
    libtorrent::sha1_hash _infoHash;

    // View model for seller peer plugin table
    QStandardItemModel _sellerPeerPluginTableViewModel;

    // Maps endpoint to view model for corresponding seller peer plugin
    // We use pointers since SellerPeerPluginViewModel are QObjects
    QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *> _sellerPeerPluginViewModels;
};

#endif // SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP
