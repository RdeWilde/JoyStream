#ifndef ADDTORRENTDIALOG_H
#define ADDTORRENTDIALOG_H

#include <QDialog>

namespace Ui {
class AddTorrentDialog;
}

class AddTorrentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTorrentDialog(QWidget *parent = 0);
    ~AddTorrentDialog();

private:
    Ui::AddTorrentDialog *ui;
};

#endif // ADDTORRENTDIALOG_H
