#include <gui/ReceiveFundsDialog.hpp>
#include "ui_ReceiveFundsDialog.h"

#include <QUrl>
#include <QDesktopServices>

ReceiveFundsDialog::ReceiveFundsDialog(QWidget * parent)
    : QDialog(parent)
    , ui(new Ui::ReceiveFundsDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->freeCoinsPushButton,
                     &QPushButton::clicked,
                     this,
                     &ReceiveFundsDialog::freeCoinsPushButtonClicked);

    // before we did
    //QUrl url("https://accounts.blockcypher.com/testnet-faucet");
    //QDesktopServices::openUrl(url);

    // Set title of dialog window
    setWindowTitle("Top up wallet");
}

ReceiveFundsDialog::~ReceiveFundsDialog()
{
    delete ui;
}

void ReceiveFundsDialog::setReceiveAddress(const Coin::P2PKHAddress & address) {
    ui->receiveAddressLineEdit->setText(address.toBase58CheckEncoding());
}

void ReceiveFundsDialog::on_closePushButton_clicked() {
    close();
}
