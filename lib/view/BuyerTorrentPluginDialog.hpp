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
class BuyerPeerPluginViewModel;

class BuyerTorrentPluginDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    BuyerTorrentPluginDialog(QWidget * parent,
                             const BuyerTorrentPluginViewModel * model);

    // Text conversion routines
    static QString pluginStateToString(BuyerTorrentPlugin::State state);
    static QString payorStateToString(Payor::State state);
    static QString contractTxIdToString(const TxId & id);
    static QString utxoToString(const UnspentP2PKHOutput & utxo);

    // Destructor
    ~BuyerTorrentPluginDialog();

    // Getters
    QStandardItemModel * channelTableViewModel();
    QStandardItemModel * buyerPeerPluginTableViewModel();
    QVector<ChannelView *> channelViews() const;
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginView *> buyerPeerPluginViews() const;

public slots:

    // Adds channels and peers,
    //void addChannel(const ChannelViewModel * model);
    void addPeer(const BuyerPeerPluginViewModel * model);
    void removePeer(const libtorrent::tcp::endpoint & endPoint);

    // Update
    void updatePluginState(BuyerTorrentPlugin::State state);
    void updatePayorState(Payor::State state);
    void updateContractTxId(const TxId & id);
    void updateUtxo(const UnspentP2PKHOutput & utxo);

private:

    Ui::BuyerTorrentDialog *ui;

    // Payor channel table view model
    QStandardItemModel _channelTableViewModel;

    // Buyer peer plugin table view model
    QStandardItemModel _buyerPeerPluginTableViewModel;

    // Views for channels
    // Pointers are owned by *this object
    QVector<ChannelView *> _channelViews;

    // Views for buyer peer plugins
    // Pointers are owned by *this object
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginView *> _buyerPeerPluginViews;

};

#endif // BUYER_TORRENT_DIALOG_HPP
