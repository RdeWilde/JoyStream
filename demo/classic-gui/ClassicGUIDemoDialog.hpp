/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef CLASSICGUIDEMODIALOG_HPP
#define CLASSICGUIDEMODIALOG_HPP

#include <QDialog>

namespace Ui {
class ClassicGUIDemoDialog;
}

namespace joystream {
namespace demo {

class ClassicGUIDemoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClassicGUIDemoDialog(QWidget *parent = 0);
    ~ClassicGUIDemoDialog();

private slots:

    void on_MainWindowPushButton_clicked();

    void on_PeersDialogPushButton_clicked();

    void on_BuyingSessionDialogPushButton_clicked();

    void on_SellingSessionDialogPushButton_clicked();

private:
    Ui::ClassicGUIDemoDialog *ui;
};

}
}

#endif // CLASSICGUIDEMODIALOG_HPP
