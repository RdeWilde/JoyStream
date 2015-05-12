#ifndef BUYER_TORRENT_DIALOG_HPP
#define BUYER_TORRENT_DIALOG_HPP

#include "extension/BuyerTorrentPlugin.hpp"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class BuyerTorrentDialog;
}

class BuyerTorrentPluginViewModel;
class ChannelView;
class BuyerPeerPluginView;

class BuyerTorrentPluginDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    BuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel * model);

    // Destructor
    ~BuyerTorrentPluginDialog();

public slots:

    // Peer plugin related event
    void addPeer();

    // Update plugin fields (though not all of these are mutable)
    void updateState(BuyerTorrentPlugin::State state);
    //void updateConfiguration(const BuyerTorrentPlugin::Configuration & configuration);
    void updateUtxo(const UnspentP2PKHOutput & utxo);



private:

    Ui::BuyerTorrentDialog *ui;

    // Payor channel table view model
    QStandardItemModel _channelTableViewModel;

    // Buyer peer plugin table view model
    QStandardItemModel _buyerPeerPluginTableViewModel;

    // Views for channels
    QVector<ChannelView *> _channelViews;

    // Views for buyer peer plugins
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginView *> _buyerPeerPluginViews;

};

#endif // BUYER_TORRENT_DIALOG_HPP
