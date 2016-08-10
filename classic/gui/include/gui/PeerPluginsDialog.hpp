/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PEERPLUGINSDIALOG_HPP
#define PEERPLUGINSDIALOG_HPP

#include <QDialog>

namespace Ui {
class PeerPluginsDialog;
}

class PeerPluginsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PeerPluginsDialog(QWidget *parent = 0);
    ~PeerPluginsDialog();

private:
    Ui::PeerPluginsDialog *ui;
};

#endif // PEERPLUGINSDIALOG_HPP
