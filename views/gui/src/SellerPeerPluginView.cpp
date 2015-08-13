/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <gui/SellerPeerPluginView.hpp>
#include <core/controller/SellerPeerPluginViewModel.hpp>
#include <common/BitcoinRepresentation.hpp>

#include <libtorrent/socket_io.hpp> // print_endpoint

#include <QStandardItem>

SellerPeerPluginView::SellerPeerPluginView(QObject * parent,
                                           const SellerPeerPluginViewModel * peerModel,
                                           const BitcoinDisplaySettings * settings,
                                           QStandardItem * endPointItem,
                                           QStandardItem * clientStateItem,
                                           QStandardItem * contractOutPointItem,
                                           QStandardItem * fundsItem,
                                           QStandardItem * refundLockTimeItem,
                                           QStandardItem * priceItem,
                                           QStandardItem * numberOfPaymentMadeItem,
                                           QStandardItem * balanceItem)
    : QObject(parent)
    , _settings(settings)
    , _endPointItem(endPointItem)
    , _clientStateItem(clientStateItem)
    , _contractOutPointItem(contractOutPointItem)
    , _fundsItem(fundsItem)
    , _refundLockTimeItem(refundLockTimeItem)
    , _priceItem(priceItem)
    , _numberOfPaymentMadeItem(numberOfPaymentMadeItem)
    , _balanceItem(balanceItem) {

    // Set data
    updateEndPoint(peerModel->endPoint());
    updateClientState(peerModel->clientState());

    const PayeeViewModel * model = peerModel->payeeViewModel();
    Payee::Status status = model->status();

    updateContractOutPointItem(status.contractOutPoint());
    updateFunds(status.funds());
    updateRefundLockTime(status.lockTime());
    updatePrice(status.price());
    updateNumberOfPaymentMade(status.numberOfPaymentsMade());
    updateBalance(status.price() * status.numberOfPaymentsMade());

    // Center content
    _endPointItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _clientStateItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _contractOutPointItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _fundsItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _refundLockTimeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _priceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _numberOfPaymentMadeItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    _balanceItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

    // Connect view model signals to slots of this view object
    QObject::connect(peerModel,
                     SIGNAL(clientStateChanged(SellerPeerPlugin::ClientState)),
                     this,
                     SLOT(updateClientState(SellerPeerPlugin::ClientState)));

    const PayeeViewModel * payeeViewModel = peerModel->payeeViewModel();

    QObject::connect(payeeViewModel,
                     SIGNAL(contractOutPointChanged(OutPoint)),
                     this,
                     SLOT(updateContractOutPointItem(OutPoint)));

    QObject::connect(payeeViewModel,
                     SIGNAL(numberOfPaymentsMadeChanged(quint64)),
                     this,
                     SLOT(updateNumberOfPaymentMade(quint64)));

    QObject::connect(payeeViewModel,
                     SIGNAL(lockTimeChanged(quint32)),
                     this,
                     SLOT(updateRefundLockTime(quint32)));

    QObject::connect(payeeViewModel,
                     SIGNAL(priceChanged(quint64)),
                     this,
                     SLOT(updatePrice(quint64)));

    QObject::connect(payeeViewModel,
                     SIGNAL(fundsChanged(quint64)),
                     this,
                     SLOT(updateFunds(quint64)));

    QObject::connect(payeeViewModel,
                     SIGNAL(balanceChanged(quint64)),
                     this,
                     SLOT(updateBalance(quint64)));

}

QString SellerPeerPluginView::endPointToString(const libtorrent::tcp::endpoint & endPoint) {

    std::string endPointString = libtorrent::print_endpoint(endPoint);
    return QString::fromStdString(endPointString);
}

QString SellerPeerPluginView::clientStateToString(SellerPeerPlugin::ClientState state) {

    QString text;

    switch(state) {

        case SellerPeerPlugin::ClientState::no_bitswapr_message_sent:
            text = "No message sent";
            break;

        case SellerPeerPlugin::ClientState::seller_mode_announced:
            text = "Announced seller mode";
            break;

        case SellerPeerPlugin::ClientState::joined_contract:
            text = "Joined contract";
            break;

        case SellerPeerPlugin::ClientState::ignored_contract_invitation:
            text = "Ignored contract";
            break;

        case SellerPeerPlugin::ClientState::signed_refund:
            text = "Signed refund";
            break;

        case SellerPeerPlugin::ClientState::ignored_sign_refund_invitation:
            text = "Ignored refund request";
            break;

        case SellerPeerPlugin::ClientState::awaiting_fullpiece_request_after_ready_announced:
            text = "Awaiting piece request";
            break;

        case SellerPeerPlugin::ClientState::awaiting_payment:
            text = "Awaiting payment ";
            break;

        case SellerPeerPlugin::ClientState::awaiting_piece_request_after_payment:
            text = "Awaiting piece request";
            break;

        case SellerPeerPlugin::ClientState::reading_piece_from_disk:
            text = "Reading from disk";
            break;
    }

    return text;
}

QString SellerPeerPluginView::contractOutPointString(const Coin::OutPoint & o) {
    return o.toString();
}

QString SellerPeerPluginView::fundsToString(quint64 funds) {
    //return QString::number(funds) + "Ƀ";
    return BitcoinRepresentation(funds).toString(_settings);
}

QString SellerPeerPluginView::refundLockTimeString(quint32 refundLockTime) {
    return QString::number(refundLockTime)+"s";
}

QString SellerPeerPluginView::priceToString(quint64 price) {
    //return QString::number(price) + "Ƀ";
    return BitcoinRepresentation(price).toString(_settings);
}

QString SellerPeerPluginView::numberOfPaymentMadeToString(quint32 numberOfPaymentMade) {
    return QString::number(numberOfPaymentMade);
}

QString SellerPeerPluginView::balanceToString(quint32 balance) {
    //return QString::number(balance) + "Ƀ";
    return BitcoinRepresentation(balance).toString(_settings);
}

void SellerPeerPluginView::updateEndPoint(const libtorrent::tcp::endpoint & endPoint) {
    _endPointItem->setText(endPointToString(endPoint));
}

void SellerPeerPluginView::updateClientState(SellerPeerPlugin::ClientState state) {
    _clientStateItem->setText(clientStateToString(state));
}

void SellerPeerPluginView::updateContractOutPointItem(const Coin::OutPoint & o) {
    _contractOutPointItem->setText(contractOutPointString(o));
}

void SellerPeerPluginView::updateFunds(quint64 funds) {
    _fundsItem->setText(fundsToString(funds));
}

void SellerPeerPluginView::updateRefundLockTime(quint32 refundLockTime) {
    _refundLockTimeItem->setText(refundLockTimeString(refundLockTime));
}

void SellerPeerPluginView::updatePrice(quint64 price) {
    _priceItem->setText(priceToString(price));
}

void SellerPeerPluginView::updateNumberOfPaymentMade(quint64 numberOfPaymentMade) {
    _numberOfPaymentMadeItem->setText(numberOfPaymentMadeToString(numberOfPaymentMade));
}

void SellerPeerPluginView::updateBalance(quint64 balance) {
    _balanceItem->setText(balanceToString(balance));
}

QStandardItem *SellerPeerPluginView::balanceItem() const {
    return _balanceItem;
}

void SellerPeerPluginView::setBalanceItem(QStandardItem *balanceItem) {
    _balanceItem = balanceItem;
}

QStandardItem *SellerPeerPluginView::numberOfPaymentMadeItem() const {
    return _numberOfPaymentMadeItem;
}

void SellerPeerPluginView::setNumberOfPaymentMadeItem(QStandardItem *numberOfPaymentMadeItem) {
    _numberOfPaymentMadeItem = numberOfPaymentMadeItem;
}

QStandardItem *SellerPeerPluginView::priceItem() const {
    return _priceItem;
}

void SellerPeerPluginView::setPriceItem(QStandardItem *priceItem) {
    _priceItem = priceItem;
}

QStandardItem *SellerPeerPluginView::refundLockTimeItem() const {
    return _refundLockTimeItem;
}

void SellerPeerPluginView::setRefundLockTimeItem(QStandardItem *refundLockTimeItem) {
    _refundLockTimeItem = refundLockTimeItem;
}

QStandardItem *SellerPeerPluginView::fundsItem() const {
    return _fundsItem;
}

void SellerPeerPluginView::setFundsItem(QStandardItem *fundsItem) {
    _fundsItem = fundsItem;
}

QStandardItem *SellerPeerPluginView::contractOutPointItem() const {
    return _contractOutPointItem;
}

void SellerPeerPluginView::setContractOutPointItem(QStandardItem *contractOutPointItem) {
    _contractOutPointItem = contractOutPointItem;
}

QStandardItem *SellerPeerPluginView::clientStateItem() const {
    return _clientStateItem;
}

void SellerPeerPluginView::setClientStateItem(QStandardItem *clientStateItem) {
    _clientStateItem = clientStateItem;
}

QStandardItem *SellerPeerPluginView::endPointItem() const {
    return _endPointItem;
}

void SellerPeerPluginView::setEndPointItem(QStandardItem *endPointItem) {
    _endPointItem = endPointItem;
}
