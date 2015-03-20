#include "BuyerTorrentPluginConfigurationDialog.hpp"
#include "ui_BuyerTorrentPluginConfigurationDialog.h"
#include "extension/PluginMode.hpp"

//#include "extension/TorrentPluginConfiguration.hpp"
#include "extension/BuyerTorrentPlugin.hpp"

#include "controller/Controller.hpp"
#include "extension/BitCoin/Wallet.hpp"B"

#include "extension/BitCoin/BitSwaprjs.hpp"
#include "extension/BitCoin/UnspentP2PKHOutput.hpp"

#include <QtMath>
#include <QMessageBox>
#include <QVector>

BuyerTorrentPluginConfigurationDialog::BuyerTorrentPluginConfigurationDialog(Controller * controller, Wallet * wallet, const libtorrent::torrent_info & torrentInfo)
    : ui(new Ui::BuyerTorrentPluginConfigurationDialog)
    , _controller(controller)
    , _wallet(wallet)
    , _torrentInfo(torrentInfo) {

    ui->setupUi(this);
}

BuyerTorrentPluginConfigurationDialog::~BuyerTorrentPluginConfigurationDialog() {
    delete ui;
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_accepted() {

    // Get max price
    quint64 maxPrice = ui->maxPriceLineEdit->text().toInt();

    // Get funding output - this has to be grabbed from wallet/chain later
    quint64 minimalFunds = _torrentInfo.num_pieces()*maxPrice;
    UnspentP2PKHOutput utxo = _wallet->getUtxo(minimalFunds, 1);

    // Check that an utxo was indeed found
    if(utxo.fundingValue() == 0) {

        // Show modal dialog on same thread, we block untill it is closed
        QMessageBox msgBox;
        msgBox.setText("No utxo found.");
        msgBox.exec();

        return;
    }

    // Figure out how much to lock up with each seller
    qint32 numberOfSellers = ui->minPeersLineEdit->text().toInt();
    quint64 fundingPerSeller = qFloor(utxo.fundingValue()/numberOfSellers);
    quint64 tx_fee = 0; // Get from where?
    quint64 changeValue = utxo.fundingValue() - numberOfSellers*fundingPerSeller - tx_fee;

    // Get other values
    QTime maxLockTime = ui->maxLockTimeEdit->time();
    quint32 maxLock = maxLockTime.hour()*3600 + maxLockTime.minute()*60 + maxLockTime.second();
    //quint64 maxFeePerByte = ui->feeLineEdit->text().toInt();

    // Use this for something?
    //QTime waitTime = ui->waitTimeTimeEdit->time();

    // Generate keys in wallet
    QList<PublicKey> buyerInContractPks = _wallet->generateNewKeys(numberOfSellers, Wallet::Purpose::BuyerInContractOutput).keys();
    QList<PublicKey> buyerFinalPks = _wallet->generateNewKeys(numberOfSellers, Wallet::Purpose::ContractFinal).keys();
    PublicKey changeOutPointPk = _wallet->generateNewKeys(1, Wallet::Purpose::ContractChange).keys()[0];

    // Create configuration
    QVector<Payor::Channel::PayorSettings> channels;
    for(int i = 0;i < numberOfSellers;i++)
        channels.append(Payor::Channel::PayorSettings(fundingPerSeller, buyerInContractPks[i], buyerFinalPks[i]));

    Payor::Configuration payorConfiguration(channels,
                                            utxo.fundingOutput(),
                                            utxo.fundingOutputKeyPair().pk(),
                                            changeOutPointPk,
                                            changeValue,
                                            maxPrice,
                                            maxLock);

    BuyerTorrentPlugin::Configuration * configuration = new BuyerTorrentPlugin::Configuration(false,
                                                                                              BuyerTorrentPlugin::State::waiting_for_payor_to_be_ready,
                                                                                              payorConfiguration);

    // Set in seller mode
    _controller->startTorrentPlugin(_torrentInfo.info_hash(), configuration);

    // close window
    done(0);
}

void BuyerTorrentPluginConfigurationDialog::on_buttonBox_rejected() {

    // Close window
    done(0);
}
