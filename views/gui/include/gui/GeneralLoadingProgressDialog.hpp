/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 17 2015
 */

#ifndef GENERAL_LOADING_PROGRESS_DIALOG_HPP
#define GENERAL_LOADING_PROGRESS_DIALOG_HPP

#include <QDialog>

namespace Ui {
class GeneralLoadingProgressDialog;
}

class GeneralLoadingProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralLoadingProgressDialog(const QString title, QWidget *parent = 0);
    ~GeneralLoadingProgressDialog();

    void updateMessage(const QString message);

private:
    Ui::GeneralLoadingProgressDialog *ui;
};

#endif // GENERAL_LOADING_PROGRESS_DIALOG_HPP
