#include "BuyerTorrentPluginDialog.hpp"
#include "ui_BuyerTorrentPluginDialog.h"

#include <libtorrent/socket_io.hpp> // print_endpoint

/**
 * BuyerTorrentDialog
 */

BuyerTorrentPluginDialog::BuyerTorrentPluginDialog(BuyerTorrentPlugin::State pluginState,
                                                   Payor::State payorState,
                                                   const TxId & id,
                                                   const UnspentP2PKHOutput & utxo)
    : ui(new Ui::BuyerTorrentDialog) {

    // Setup Qt ui
    ui->setupUi(this);

    // Main fields
    updatePluginState(pluginState);
    updatePayorState(payorState);
    updateContractTxId(id);
    updateUtxo(utxo);

    // Add columns to channel table view model
    QStringList channelTableColumnNames;

    channelTableColumnNames << "Index"
                            << "State"
                            << "Funds"
                            << "Lock"
                            << "Price"
                            << "#Payments"
                            << "Balance";

    _channelTableViewModel.setHorizontalHeaderLabels(channelTableColumnNames);

    // Add columns to peer plugin table view model
    QStringList buyerPeerPluginTableColumnNames;
    buyerPeerPluginTableColumnNames << "Host"
                                    << "State"
                                    << "Slot";

    _buyerPeerPluginTableViewModel.setHorizontalHeaderLabels(buyerPeerPluginTableColumnNames);
}

QString BuyerTorrentPluginDialog::pluginStateToString(BuyerTorrentPlugin::State state) {

    QString text;

    switch(state) {

        case BuyerTorrentPlugin::State::waiting_for_payor_to_be_ready:
            text = "Contract being constructed";
            break;
        case BuyerTorrentPlugin::State::downloading_pieces:
            text = "Downloading";
            break;
        case BuyerTorrentPlugin::State::done:
            text = "Finished";
            break;
    }

    return text;
}

QString BuyerTorrentPluginDialog::payorStateToString(Payor::State state) {

    QString text;

    switch(state) {

        case Payor::State::waiting_for_full_set_of_sellers:
            text = "Waiting for sellers";
            break;

        case Payor::State::waiting_for_full_set_of_refund_signatures:
            text = "Waiting for all refunds";
            break;

        case Payor::State::can_pay:
            text = "Can pay on channel(s)";
            break;

        case Payor::State::all_contract_outputs_spent:
            text = "All channels closed";
            break;
    }

    return text;
}

QString BuyerTorrentPluginDialog::contractTxIdToString(const TxId & id) {
    return id.toString();
}

QString BuyerTorrentPluginDialog::utxoToString(const UnspentP2PKHOutput & utxo) {
    return utxo.outPoint().toString();
}

BuyerTorrentPluginDialog::~BuyerTorrentPluginDialog() {

    delete ui;

    for(QVector<ChannelView *>::const_iterator
        i = _channelViews.constBegin(),
        end = _channelViews.constEnd();
        i != end;i++)
        delete (*i);

    for(QMap<libtorrent::tcp::endpoint, BuyerPeerPluginView *>::const_iterator
        i = _buyerPeerPluginViews.constBegin(),
        end = _buyerPeerPluginViews.constEnd();
        i != end;i++)
        delete i.value();
}

void BuyerTorrentPluginDialog::addChannel(ChannelView * view) {
    _channelViews.append(view);
}

void BuyerTorrentPluginDialog::addPeer(const libtorrent::tcp::endpoint & endPoint, BuyerPeerPluginView * view) {

    Q_ASSERT(!_buyerPeerPluginViews.contains(endPoint));

    _buyerPeerPluginViews[endPoint] = view;
}

void BuyerTorrentPluginDialog::updatePluginState(BuyerTorrentPlugin::State state) {
    ui->extensionStateLabel->setText(pluginStateToString(state));
}

void BuyerTorrentPluginDialog::updatePayorState(Payor::State state) {
    ui->stateFieldLabel->setText(payorStateToString(state));
}

void BuyerTorrentPluginDialog::updateContractTxId(const TxId & id) {
    ui->contractTxIdLabel->setText(contractTxIdToString(id));
}

void BuyerTorrentPluginDialog::updateUtxo(const UnspentP2PKHOutput & utxo) {
    ui->utxoLabel->setText(utxoToString(utxo));
}

QStandardItemModel * BuyerTorrentPluginDialog::channelTableViewModel() {
    return &_channelTableViewModel;
}

QStandardItemModel * BuyerTorrentPluginDialog::buyerPeerPluginTableViewModel() {
    return &_buyerPeerPluginTableViewModel;
}

QVector<ChannelView *> BuyerTorrentPluginDialog::channelViews() const {
    return _channelViews;
}

QMap<libtorrent::tcp::endpoint, BuyerPeerPluginView *> BuyerTorrentPluginDialog::buyerPeerPluginViews() const {
    return _buyerPeerPluginViews;
}
