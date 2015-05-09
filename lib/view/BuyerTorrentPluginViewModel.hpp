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

    // Constructor
    BuyerTorrentPluginViewModel(TorrentViewModel * parent, const BuyerTorrentPlugin::Configuration & configuration, const UnspentP2PKHOutput & utxo);

    // Add a model view for a new buyer peer plugin
    void addPeer(const libtorrent::tcp::endpoint & endPoint);

    // Update status
    void update(const BuyerTorrentPlugin::Status & status);
    void updatePeer(const libtorrent::tcp::endpoint & endPoint, const BuyerPeerPlugin::Status & status);

    // Getters and setters
    BuyerTorrentPlugin::State state() const;
    BuyerTorrentPlugin::Configuration configuration() const;
    UnspentP2PKHOutput utxo() const;
    const PayorViewModel * payorViewModel() const;

    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> buyerPeerPluginViewModels() const;
    void setBuyerPeerPluginViewModels(const QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> & buyerPeerPluginViewModels);

public slots:

signals:

    void stateChanged(BuyerTorrentPlugin::State state);

private:

    // Present state of plugin
    BuyerTorrentPlugin::State _state;

    // Configurations used for plugin at present
    BuyerTorrentPlugin::Configuration _configuration;

    // Utxo funding contract
    UnspentP2PKHOutput _utxo;

    // View model of payor
    PayorViewModel _payorViewModel;

    // Maps endpoint to view model for corresponding seller buyer plugin
    // ** We use pointers here since they are QObjects, which cannot be copied **
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> _buyerPeerPluginViewModels;
};

#endif // BUYER_TORRENT_PLUGIN_VIEW_MODEL_HPP
