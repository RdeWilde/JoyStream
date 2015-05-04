#include "PeerPluginsDialog.hpp"
#include "ui_PeerPluginsDialognsdialog.h"

PeerPluginsDialog::PeerPluginsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PeerPluginsDialog)
{
    ui->setupUi(this);
}

PeerPluginsDialog::~PeerPluginsDialog()
{
    delete ui;
}
