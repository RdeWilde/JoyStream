/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 20 2016
 */

#include <gui/SessionDialog/BuyingModeSessionWidget.hpp>
#include "ui_BuyingModeSessionWidget.h"
#include <gui/Common.hpp>
#include <protocol_wire/protocol_wire.hpp>
#include <protocol_session/protocol_session.hpp>
#include <common/BitcoinDisplaySettings.hpp>
#include <common/BitcoinRepresentation.hpp>
#include <CoinCore/CoinNodeData.h>

#include <QStandardItemModel>

namespace joystream {
namespace classic {
namespace gui {

BuyingModeSessionWidget::BuyingModeSessionWidget(QWidget *parent,
                                                 const BitcoinDisplaySettings * settings)
    : QWidget(parent)
    , ui(new Ui::Form())
    , _settings(settings) {

    ui->setupUi(this);

    //ui->stateValueLabel->setText("");
    //ui->contractTxIdValueLabel->setText("");
    //ui->updateTermsPushButton->hide(); // not not implemented

    QObject::connect(ui->updateTermsPushButton, &QPushButton::clicked,
                     this, &BuyingModeSessionWidget::updateTermsClicked);
}

BuyingModeSessionWidget::~BuyingModeSessionWidget() {
    delete ui;
}

void BuyingModeSessionWidget::setSellerTreeViewModel(QAbstractItemModel * model) {
    ui->sellersTableView->setModel(model);
}

void BuyingModeSessionWidget::setTerms(const protocol_wire::BuyerTerms & terms) {

    ui->maxContractFeePrKbValueLabel->setText(BitcoinRepresentation(terms.maxContractFeePerKb()).toString(_settings));

    ui->minNumberOfSellersValueLabel->setText(QString::number(terms.minNumberOfSellers()));

    ui->maxPriceValueLabel->setText(BitcoinRepresentation(terms.maxPrice()).toString(_settings));

    ui->maxRefundDelayValueLabel->setText(QString::number(terms.maxLock()));
}

void BuyingModeSessionWidget::setPolicy(const protocol_session::BuyingPolicy &) {
    // nothing to do really
}

void BuyingModeSessionWidget::setState(const protocol_session::BuyingState & state) {

    ui->stateValueLabel->setText(Common::toString(state));

    // Set update terms button visibility
    if(state != protocol_session::BuyingState::sending_invitations)
        ui->updateTermsPushButton->hide();
    else
        ui->updateTermsPushButton->show();
}

void BuyingModeSessionWidget::setContractTx(const Coin::Transaction & tx) {

    std::string hexEncodedTxId = tx.getHashLittleEndian().getHex();

    ui->contractTxIdValueLabel->setText(QString::fromStdString(hexEncodedTxId));
}

}
}
}
