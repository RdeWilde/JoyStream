/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#include "ClassicGUIDemoDialog.hpp"
#include "ui_ClassicGUIDemoDialog.h"
#include <gui/MainWindow.hpp>

namespace joystream {
namespace demo {

ClassicGUIDemoDialog::ClassicGUIDemoDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ClassicGUIDemoDialog) {

    ui->setupUi(this);
}

ClassicGUIDemoDialog::~ClassicGUIDemoDialog() {
    delete ui;
}

void ClassicGUIDemoDialog::on_pushButton_clicked() {

    BitcoinDisplaySettings settings;

    classic::gui::MainWindow window("JoyStream Classic GUI Demo",
                                    0,
                                    0,
                                    0,
                                    &settings);
    window.show();

    QEventLoop loop;
    QObject::connect(&window,
                     &classic::gui::MainWindow::ignoredCloseEventOccured,
                     &loop,
                     &QEventLoop::quit);

    loop.exec();
}

}
}
