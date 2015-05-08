#ifndef BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP
#define BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP

//#include "TorrentPluginViewModel.hpp
#include "extension/BuyerPeerPlugin.hpp" // BuyerTorrentPlugin::Configuration
#include "extension/BuyerTorrentPlugin.hpp"

#include "PayorViewModel.hpp"

#include <libtorrent/socket_io.hpp>

#include <QStandardItemModel>

class BuyerPeerPluginViewModel;
class TorrentViewModel;

class BuyerTorrentPluginViewModel : public QObject { // : public TorrentPluginViewModel {

    Q_OBJECT

public:

    static const char * columnTitles[];
    static const int numberOfColumns;

    // Constructor from members
    //BuyerTorrentPluginViewModel(TorrentViewModel * parent, const libtorrent::sha1_hash & infoHash, const BuyerTorrentPlugin::Configuration & configuration);
    BuyerTorrentPluginViewModel(TorrentViewModel * parent, const BuyerTorrentPlugin::Configuration & configuration);

    // Add a model view for a new buyer peer plugin
    void addPeer(const libtorrent::tcp::endpoint & endPoint);

    // Update status
    void update(const BuyerTorrentPlugin::Status & status);
    void updatePeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status);

    // Getters and setters
    QStandardItemModel * buyerPeerPluginTableViewModel();

    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> buyerPeerPluginViewModels() const;
    void setBuyerPeerPluginViewModels(const QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> & buyerPeerPluginViewModels);

public slots:

signals:

private:

    // Info hash of corresponding torrent
    //libtorrent::sha1_hash _infoHash;

    // Configurations used for plugin at present
    BuyerTorrentPlugin::Configuration _configuration;

    // Buyer peer plugin table view model
    QStandardItemModel _buyerPeerPluginTableViewModel;

    // View model of payor
    PayorViewModel _payorViewModel;

    // Maps endpoint to view model for corresponding seller buyer plugin
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> _buyerPeerPluginViewModels;
};


//#include <QMetaType>
//Q_DECLARE_METATYPE(const BuyerTorrentPluginViewModel *)

#endif // BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP
