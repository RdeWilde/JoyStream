/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#include <gui/SessionDialog/SellingModeSessionWidget.hpp>
#include "ui_SellingModeSessionWidget.h"
#include <gui/Common.hpp>
#include <common/BitcoinRepresentation.hpp>
#include <protocol_wire/protocol_wire.hpp>

#include <QStandardItemModel>

namespace joystream {
namespace classic {
namespace gui {

SellingModeSessionWidget::SellingModeSessionWidget(QWidget * parent,
                                                   const BitcoinDisplaySettings * settings)
    : QWidget(parent)
    , ui(new Ui::SellingModeSessionWidget)
    , _settings(settings) {

    ui->setupUi(this);
}

SellingModeSessionWidget::~SellingModeSessionWidget() {
    delete ui;
}

void SellingModeSessionWidget::setBuyerTreeViewModel(QStandardItemModel * model) {
    ui->buyersTableView->setModel(model);
}

void SellingModeSessionWidget::setTerms(const protocol_wire::SellerTerms & terms) {

    ui->minContractFeePrKbValueLabel->setText(BitcoinRepresentation(terms.minContractFeePerKb()).toString(_settings));

    ui->maxNumberOfSellersValueLabel->setText(QString::number(terms.maxSellers()));

    ui->minPriceValueLabel->setText(BitcoinRepresentation(terms.minPrice()).toString(_settings));

    ui->minRefundDelayValueLabel->setText(QString::number(terms.minLock()));
}

void SellingModeSessionWidget::setPolicy(const protocol_session::SellingPolicy &) {
    // nothing to do really
}

}
}
}
