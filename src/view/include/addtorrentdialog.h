#ifndef ADDTORRENTDIALOG_H
#define ADDTORRENTDIALOG_H

#include <QDialog>
#include <QString>

#include "controller/include/Controller.hpp"

namespace Ui {
class AddTorrentDialog;
}

class AddTorrentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTorrentDialog(QWidget *parent = 0);
    ~AddTorrentDialog();

    AddTorrentDialog(Controller * controller, const QString & torrentFileName);

    void closeWindow();

private slots:
    void on_AddTorrentDialog_accepted();

    void on_AddTorrentDialog_rejected();

    void on_saveToFolderPushButton_clicked();

private:
    Ui::AddTorrentDialog * ui;

    Controller * controller_;

    const QString torrentFileName_;

    //
    void addTorrent(const QString & torrentFileName, const QString & downloadFolder);
};

#endif // ADDTORRENTDIALOG_H
