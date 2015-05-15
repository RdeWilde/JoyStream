#include "BuyerTorrentPluginDialog.hpp"
#include "ui_BuyerTorrentPluginDialog.h"

#include "controller/BuyerTorrentPluginViewModel.hpp"
#include "controller/BuyerPeerPluginViewModel.hpp"
#include "ChannelView.hpp"
#include "BuyerPeerPluginView.hpp"

#include <libtorrent/socket_io.hpp> // print_endpoint

/**
 * BuyerTorrentDialog
 */

BuyerTorrentPluginDialog::BuyerTorrentPluginDialog(QWidget * parent,
                                                   const BuyerTorrentPluginViewModel * model)
    : QDialog(parent)
    , ui(new Ui::BuyerTorrentDialog) {

    // Setup Qt ui
    ui->setupUi(this);

    const PayorViewModel * payorViewModel = model->payorViewModel();

    // Main fields
    updatePluginState(model->state());

    updatePayorState(payorViewModel->state());
    updateContractTxId(payorViewModel->contractTxId());
    updateUtxo(payorViewModel->utxo());

    /**
     * Connect model signals to view slots
     */

    // addChannel, in future

    QObject::connect(model,
                     SIGNAL(peerAdded(libtorrent::tcp::endpoint,const BuyerPeerPluginViewModel*)),
                     this,
                     SLOT(addPeer(libtorrent::tcp::endpoint,const BuyerPeerPluginView*)));

    QObject::connect(model,
                     SIGNAL(stateChanged(BuyerTorrentPlugin::State)),
                     this,
                     SLOT(updatePluginState(BuyerTorrentPlugin::State)));

    QObject::connect(payorViewModel,
                     SIGNAL(stateChanged(Payor::State)),
                     this,
                     SLOT(updatePayorState(Payor::State)));

    QObject::connect(payorViewModel,
                     SIGNAL(contractTxIdChanged(TxId)),
                     this,
                     SLOT(updateContractTxId(TxId)));

    QObject::connect(payorViewModel,
                     SIGNAL(utxoChanged(UnspentP2PKHOutput)),
                     this,
                     SLOT(updateUtxo(UnspentP2PKHOutput)));

    /**
     * Channels
     */

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

    // Create channel views
    QVector<ChannelViewModel *> channelViewModels = payorViewModel->channelViewModels();

    for(QVector<ChannelViewModel *>::const_iterator
        i = channelViewModels.constBegin(),
        end = channelViewModels.constEnd();
        i != end;i++)
        _channelViews.append(new ChannelView(this, *i, &_channelTableViewModel));

    // Set model for channels table
    ui->channelsTableView->setModel(&_channelTableViewModel);

    /**
     * Peer plugins
     */
    // Add columns to peer plugin table view model
    QStringList buyerPeerPluginTableColumnNames;
    buyerPeerPluginTableColumnNames << "Host"
                                    << "State"
                                    << "Slot";

    _buyerPeerPluginTableViewModel.setHorizontalHeaderLabels(buyerPeerPluginTableColumnNames);

    // Create peer plugin views
    QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *> peerViewModels = model->buyerPeerPluginViewModels();

    for(QMap<libtorrent::tcp::endpoint, BuyerPeerPluginViewModel *>::const_iterator
        i = peerViewModels.constBegin(),
        end = peerViewModels.constEnd();
        i != end;i++)
        addPeer(i.value());

    // Set model for channels table
    ui->peerPluginsTableView->setModel(&_buyerPeerPluginTableViewModel);
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

    /**
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
    */
}

void BuyerTorrentPluginDialog::addPeer(const BuyerPeerPluginViewModel * model) {

    libtorrent::tcp::endpoint endPoint = model->endPoint();

    Q_ASSERT(!_buyerPeerPluginViews.contains(endPoint));

    _buyerPeerPluginViews[endPoint] = new BuyerPeerPluginView(this, model, &_buyerPeerPluginTableViewModel);
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
