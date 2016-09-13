/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 13 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_SELECTTORRENTFORDOWNLOADDIALOG_HPP
#define JOYSTREAM_CLASSIC_GUI_SELECTTORRENTFORDOWNLOADDIALOG_HPP

#include <QDialog>

namespace Ui {
class SelectTorrentForDownloadDialog;
}

namespace joystream {
namespace classic {
namespace gui {

class SelectTorrentForDownloadDialog : public QDialog
{
    Q_OBJECT

public:

    enum class Selected {
        None,
        TorrentFile,
        MagnetLink
    };

    explicit SelectTorrentForDownloadDialog(QWidget *parent = 0);
    ~SelectTorrentForDownloadDialog();

    Selected selected() const noexcept;

private slots:

    void on_TorrentFilePushButton_clicked();

    void on_MagnetLinkPushButton_clicked();

private:
    Ui::SelectTorrentForDownloadDialog *ui;

    Selected _selected;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_SELECTTORRENTFORDOWNLOADDIALOG_HPP
