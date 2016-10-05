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

    ui->setupUi(this);
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

    //_sellingWidget = widget;

    // Adjust widget
    widget->setParent(this);
    widget->setGeometry(10, 40, widget->width(), widget->height());
    widget->show();

    // Adjust dialog size: use layout later
    setGeometry(QRect(this->geometry().left(), this->geometry().top(), widget->width() + 20, 40 + widget->height() + 20));
}

void SessionDialog::showBuyModeWidget(BuyingModeSessionWidget * widget) {

    ui->sessionModeValueLabel->setText(Common::toString(protocol_session::SessionMode::buying));

    if(_sellingWidget != nullptr) {
        delete _sellingWidget;
        _sellingWidget = nullptr;
    }

    _buyingWidget = widget;

    // Adjust widget
    widget->setParent(this);
    widget->setGeometry(10, 40, widget->width(), widget->height());
    widget->show();

    // Adjust dialog size: use layout later
    setGeometry(QRect(this->geometry().left(), this->geometry().top(), widget->width() + 20, 40 + widget->height() + 20));
}

}
}
}
