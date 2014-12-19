#include "BuyerTorrentPluginConfigurationDialog.hpp"
#include "ui_BuyerTorrentPluginConfigurationDialog.h"
#include "extension/StartedPluginMode.hpp"
#include "extension/TorrentPluginConfiguration.hpp"
#include "controller/Controller.hpp"

/*
BuyerTorrentPluginConfigurationDialog::BuyerTorrentPluginConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuyerTorrentPluginConfigurationDialog)
{
    ui->setupUi(this);
}
*/

BuyerTorrentPluginConfigurationDialog::BuyerTorrentPluginConfigurationDialog(Controller * controller, const libtorrent::sha1_hash & infoHash)
    : ui(new Ui::BuyerTorrentPluginConfigurationDialog)
    , _controller(controller)
    , _infoHash(infoHash) {

    ui->setupUi(this);
}

BuyerTorrentPluginConfigurationDialog::~BuyerTorrentPluginConfigurationDialog()
{
    delete ui;
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_accepted()
{

    // Set in seller mode
    _controller->updateTorrentPluginConfiguration(_infoHash, new TorrentPluginConfiguration(StartedPluginMode::Seller, true));

    // close window
    done(0);
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_rejected() {

    // Close window
    done(0);
}
