/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 22 2016
 */

#include <gui/SessionDialog/SessionDialog.hpp>
#include <gui/SessionDialog/SellingModeSessionWidget.hpp>
#include <gui/SessionDialog/BuyingModeSessionWidget.hpp>
#include <gui/Common.hpp>
#include "ui_SessionDialog.h"
#include <core/core.hpp>

namespace joystream {
namespace classic {
namespace gui {

SessionDialog::SessionDialog(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::SessionDialog)
    , _sellingWidget(nullptr)
    , _buyingWidget(nullptr){
}

SessionDialog::~SessionDialog()
{
    delete ui;
}

void SessionDialog::showSellModeWidget(SellingModeSessionWidget * widget) {

    ui->sessionModeValueLabel->setText(Common::toString(protocol_session::SessionMode::selling));

    if(_buyingWidget != nullptr) {
        delete _buyingWidget;
        _buyingWidget =  nullptr;
    }

    _sellingWidget = widget;

    // Position widget

    // Resize dialog

}

void SessionDialog::showBuyModeWidget(BuyingModeSessionWidget * widget) {

    ui->sessionModeValueLabel->setText(Common::toString(protocol_session::SessionMode::buying));

    if(_sellingWidget != nullptr) {
        delete _sellingWidget;
        _sellingWidget = nullptr;
    }

    _buyingWidget = widget;

    // Position widget

    // Resize dialog

}

}
}
}
