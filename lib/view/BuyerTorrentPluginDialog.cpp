#include "BuyerTorrentPluginDialog.hpp"
#include "ui_BuyerTorrentDialog.h"
#include "controller/BuyerTorrentPluginViewModel.hpp"

#include <libtorrent/socket_io.hpp> // print_endpoint

/**
 * BuyerTorrentDialog
 */

BuyerTorrentPluginDialog::BuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel * model)
    : //QDialog(parent)
    ui(new Ui::BuyerTorrentDialog) {

    // Setup Qt ui
    ui->setupUi(this);

    /**
     * Shallow fields
     */
    //updateState(model->state());
    //updateConfiguration(model->configuration());
    //updateUtxo(model->utxo());

    /**
     * Payor

    // Add columns to channel table view model
    QStringList channelTableColumnNames;

    channelTableColumnNames << "Host"
                            << "Index"
                            << "State"
                            << "Funds"
                            << "Lock"
                            << "Price"
                            << "#Payments"
                            << "Balance";

    _channelTableViewModel.setHorizontalHeaderLabels(channelTableColumnNames);

    // Add rows to table view model for each channel
    const PayorViewModel * payorViewModel = model->payorViewModel();

    QVector<ChannelViewModel *> channelViewModels = payorViewModel->channelViewModels();

    for(QVector<ChannelViewModel *>::const_iterator
        i = channelViewModels.constBegin(),
        end = channelViewModels.constEnd(); i != end;i++) {

        // Get channel view model
        const ChannelViewModel * channelViewModel = *i;

        // Build row of items
        QList<QStandardItem *> items;

        items << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem();

        // Add row to model
        _channelTableViewModel.appendRow(items);

        // Get channel configuration and update initial values
        Payor::Channel::Configuration configuration = channelViewModel.configuration();

        updateState(configuration.index(), configuration.state());
        updateFunds(configuration.index(), configuration.funds());
        updateRefundLockTime(configuration.index(), configuration.refundLockTime());
        updatePrice(configuration.index(), configuration.price());
        updateNumberOfPaymentsMade(configuration.index(), configuration.numberOfPaymentsMade());
        updateBalance(configuration.index(), configuration.price()*configuration.numberOfPaymentsMade());

        // Connect channel view model signals to slots
        QObject::connect(channelViewModel,
                         SIGNAL(stateChanged(quint32,Payor::Channel::State)),
                         this,
                         SLOT(updateState(quint32,Payor::Channel::State)));

        QObject::connect(channelViewModel,
                         SIGNAL(fundsChanged(quint32,quint64)),
                         this,
                         SLOT(updateFunds(quint32,quint64));

        QObject::connect(channelViewModel,
                         SIGNAL(refundLockTimeChanged(quint32,quint32)),
                         this,
                         SLOT(updateRefundLockTime(quint32,quint32));

        QObject::connect(channelViewModel,
                         SIGNAL(priceChanged(quint32,quint64)),
                         this,
                         SLOT(updatePrice(quint32,quint64));

        QObject::connect(channelViewModel,
                         SIGNAL(numberOfPaymentsMadeChanged(quint32,quint64)),
                         this,
                         SLOT(updateNumberOfPaymentsMade(quint32,quint64)));

        QObject::connect(channelViewModel,
                         SIGNAL(balanceChanged(quint32,quint64)),
                         this,
                         SLOT(updateBalance(quint32,quint64));
    }

    */

    /**
     * Buyer peer plugins


    // Add columns to peer plugin table view model
    QStringList buyerPeerPluginTableColumnNames;
    buyerPeerPluginTableColumnNames << "Host"
                                    << "State";

    _buyerPeerPluginTableViewModel.setHorizontalHeaderLabels(buyerPeerPluginTableColumnNames);
*/

    /**
    // Add rows to table view model for each channel
    const PayorViewModel * payorViewModel = model->payorViewModel();

    QVector<ChannelViewModel *> channelViewModels = payorViewModel->channelViewModels();

    for(QVector<ChannelViewModel *>::const_iterator
        i = channelViewModels.constBegin(),
        end = channelViewModels.constEnd(); i != end;i++) {

        // Get channel view model
        const ChannelViewModel * channelViewModel = *i;

        // Build row of items
        QList<QStandardItem *> items;

        items << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem()
              << new QStandardItem();

        // Add row to model
        _channelTableViewModel.appendRow(items);

        // Get channel configuration and update initial values
        Payor::Channel::Configuration configuration = channelViewModel.configuration();

        updateState(configuration.index(), configuration.state());
        updateFunds(configuration.index(), configuration.funds());
        updateRefundLockTime(configuration.index(), configuration.refundLockTime());
        updatePrice(configuration.index(), configuration.price());
        updateNumberOfPaymentsMade(configuration.index(), configuration.numberOfPaymentsMade());
        updateBalance(configuration.index(), configuration.price()*configuration.numberOfPaymentsMade());

        // Connect channel view model signals to slots
        QObject::connect(channelViewModel,
                         SIGNAL(stateChanged(quint32,Payor::Channel::State)),
                         this,
                         SLOT(updateState(quint32,Payor::Channel::State)));

        QObject::connect(channelViewModel,
                         SIGNAL(fundsChanged(quint32,quint64)),
                         this,
                         SLOT(updateFunds(quint32,quint64)));

        QObject::connect(channelViewModel,
                         SIGNAL(refundLockTimeChanged(quint32,quint32)),
                         this,
                         SLOT(updateRefundLockTime(quint32,quint32)));

        QObject::connect(channelViewModel,
                         SIGNAL(priceChanged(quint32,quint64)),
                         this,
                         SLOT(updatePrice(quint32,quint64)));

        QObject::connect(channelViewModel,
                         SIGNAL(numberOfPaymentsMadeChanged(quint32,quint64)),
                         this,
                         SLOT(updateNumberOfPaymentsMade(quint32,quint64)));

        QObject::connect(channelViewModel,
                         SIGNAL(balanceChanged(quint32,quint64)),
                         this,
                         SLOT(updateBalance(quint32,quint64)));
    }
    */
}

BuyerTorrentPluginDialog::~BuyerTorrentPluginDialog() {
    delete ui;
}

void BuyerTorrentPluginDialog::updateState(BuyerTorrentPlugin::State state) {

}

void BuyerTorrentPluginDialog::updateConfiguration(const BuyerTorrentPlugin::Configuration & configuration){

}

void BuyerTorrentPluginDialog::updateUtxo(const UnspentP2PKHOutput & utxo) {

}

/**
void BuyerTorrentPluginDialog::assign(quint32 index, libtorrent::tcp::endpoint & endPoint) {

}

void BuyerTorrentPluginDialog::unassigned(quint32 index) {

}
*/

void BuyerTorrentPluginDialog::updateState(quint32 index, Payor::Channel::State state) {

}

void BuyerTorrentPluginDialog::updateFunds(quint32 index, quint64 funds) {

}

void BuyerTorrentPluginDialog::updateRefundLockTime(quint32 index, quint32 refundLockTime) {

}

void BuyerTorrentPluginDialog::updatePrice(quint32 index, quint64 price) {

}

void BuyerTorrentPluginDialog::updateNumberOfPaymentsMade(quint32 index, quint64 numberOfPaymentsMade) {

}

void BuyerTorrentPluginDialog::updateBalance(quint32 index, quint64 balance) {

}

/**
void ChannelViewModel::updateHostItem(libtorrent::tcp::endpoint & endPoint) {

    std::string endPointString = libtorrent::print_endpoint(endPoint);
    _hostItem->setText(QString::fromStdString(endPointString));
}

void ChannelViewModel::updateIndexItem(quint32 index) {
    _indexItem->setText(QString::number(index));
}

void ChannelViewModel::updateStateItem(Payor::Channel::State state) {

    // Get text representation of state
    QString channelState;

    switch(state) {

        case Payor::Channel::State::unassigned:
            channelState = "Unassigned";
            break;

        case Payor::Channel::State::assigned:
            channelState = "Assigned";
            break;

        case Payor::Channel::State::refund_signed:
            channelState = "Refund signed";
            break;
    }

    // Update model view
    _stateItem->setText(channelState);
}

void ChannelViewModel::updateFundsItem(quint64 funds) {
    _fundsItem->setText(QString::number(funds));
}

void ChannelViewModel::updateRefundLockTime(quint32 refundLockTime) {
    _refundLockTimeItem->setText(QString::number(refundLockTime));
}

void ChannelViewModel::updatePriceItem(quint64 price) {
    _priceItem->setText(QString::number(price));
}

void ChannelViewModel::updateNumberOfPaymentsMadeItem(quint64 numberOfPaymentsMade) {
    _numberOfPaymentsMadeItem->setText(QString::number(numberOfPaymentsMade));
}

void ChannelViewModel::updateBalanceItem(quint64 balance) {
    _balanceItem->setText(QString::number(balance));
}

void ChannelViewModel::updateUploadSpeedItem(quint64 bytesPrSecond) {
    _uploadSpeedItem->setText(QString::number(bytesPrSecond));
}


void BuyerPeerPluginViewModel::clientStateChanged(BuyerPeerPlugin::ClientState state) {

    // Get text representation of state
    QString clientState;

    switch(state) {

        case BuyerPeerPlugin::ClientState::no_bitswapr_message_sent:
            clientState = "No message sent";
            break;

        case BuyerPeerPlugin::ClientState::buyer_mode_announced:
            clientState = "Announced buyer mode";
            break;

        case BuyerPeerPlugin::ClientState::invited_to_contract:
            clientState = "Invited to contract";
            break;

        case BuyerPeerPlugin::ClientState::ignored_join_contract_from_peer:
            clientState = "Ignored join contract";
            break;

        case BuyerPeerPlugin::ClientState::asked_for_refund_signature:
            clientState = "Requested refund";
            break;

        case BuyerPeerPlugin::ClientState::received_valid_refund_signature_and_waiting_for_others:
            clientState = "Received valid refund";
            break;

        case BuyerPeerPlugin::ClientState::needs_to_be_assigned_piece:
            clientState = "Needs to be assigned piece";
            break;

        case BuyerPeerPlugin::ClientState::waiting_for_full_piece:
            clientState = "Waiting for piece ";
            break;

        case BuyerPeerPlugin::ClientState::waiting_for_libtorrent_to_validate_piece:
            clientState = "Validating piece";
            break;
    }

    // Update model view
    _stateItem->setText(clientState);
}
*/
