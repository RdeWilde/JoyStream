#include "view/include/addtorrentdialog.h"
#include "ui_addtorrentdialog.h"

AddTorrentDialog::AddTorrentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTorrentDialog)
{
    ui->setupUi(this);
}

AddTorrentDialog::~AddTorrentDialog()
{
    delete ui;
}
