#ifndef BUYER_TORRENT_DIALOG_HPP
#define BUYER_TORRENT_DIALOG_HPP

namespace Ui {
class BuyerTorrentDialog;
}

#include "extension/BuyerTorrentPlugin.hpp"

//#include <libtorrent/socket.hpp> // tcp::endpoint

#include <QDialog>
#include <QStandardItemModel>


class BuyerTorrentPluginViewModel;

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
    void updateConfiguration(const BuyerTorrentPlugin::Configuration & configuration);
    void updateUtxo(const UnspentP2PKHOutput & utxo);


    // Update channel status fields
    void updateState(quint32 index, Payor::Channel::State state);
    void updateFunds(quint32 index, quint64 funds);
    void updateRefundLockTime(quint32 index, quint32 refundLockTime);
    void updatePrice(quint32 index, quint64 price);
    void updateNumberOfPaymentsMade(quint32 index, quint64 numberOfPaymentsMade);
    void updateBalance(quint32 index, quint64 balance);

private:

    Ui::BuyerTorrentDialog *ui;

    // Payor channel table view model
    QStandardItemModel _channelTableViewModel;

    // Buyer peer plugin table view model
    QStandardItemModel _buyerPeerPluginTableViewModel;

};

#endif // BUYER_TORRENT_DIALOG_HPP
