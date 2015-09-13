#include <gui/ReceiveFundsDialog.hpp>
#include "ui_ReceiveFundsDialog.h"
#include <wallet/Manager.hpp>

#include <QDesktopServices>

ReceiveFundsDialog::ReceiveFundsDialog(Wallet::Manager * wallet)
    : ui(new Ui::ReceiveFundsDialog)
    , _wallet(wallet)
    , _receiveAddres(_wallet->getReceiveAddress())
{
    ui->setupUi(this);

    // Set input field to contain receivea ddress
    ui->receiveAddressLineEdit->setText(_receiveAddres.toBase58CheckEncoding());

    // Set title of dialog window
    setWindowTitle("Top up wallet");
}

ReceiveFundsDialog::~ReceiveFundsDialog()
{
    delete ui;
}


void ReceiveFundsDialog::on_freeCoinsPushButton_clicked() {

    QUrl url("https://accounts.blockcypher.com/testnet-faucet");
    QDesktopServices::openUrl(url);
}

void ReceiveFundsDialog::on_closePushButton_clicked() {
    close();
}
