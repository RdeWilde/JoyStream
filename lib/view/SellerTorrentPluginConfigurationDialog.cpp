#include "SellerTorrentPluginConfigurationDialog.hpp"
#include "ui_SellerTorrentPluginConfigurationDialog.h"

#include "controller/Controller.hpp"
#include "extension/TorrentPluginConfiguration.hpp"

/*
SellerTorrentPluginConfigurationDialog::SellerTorrentPluginConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SellerTorrentPluginConfigurationDialog)
{
    ui->setupUi(this);
}
*/

SellerTorrentPluginConfigurationDialog::SellerTorrentPluginConfigurationDialog(Controller * controller, const libtorrent::sha1_hash & infoHash)
    :ui(new Ui::SellerTorrentPluginConfigurationDialog)
    ,_controller(controller)
    ,_infoHash(infoHash) {
    ui->setupUi(this);
}

SellerTorrentPluginConfigurationDialog::~SellerTorrentPluginConfigurationDialog() {
    delete ui;
}

void SellerTorrentPluginConfigurationDialog::on_buttonBox_accepted() {

    // Grab fields
    quint32 price = ui->priceLineEdit->text().toInt();

    // Create configuration
    TorrentPluginConfiguration * configuration = new TorrentPluginConfiguration(true, price);

    // Set in seller mode
    _controller->updateTorrentPluginConfiguration(_infoHash, configuration);

    // close window
    done(0);
}
