#include "WalletDialog.hpp"
#include "ui_walletdialog.h"

//WalletDialog::WalletDialog(QWidget *parent, Wallet * wallet) {}

WalletDialog::WalletDialog(Wallet * wallet) :
    ui(new Ui::WalletDialog),
    _wallet(wallet) {
    ui->setupUi(this);

    // Setup View model
    this->setWindowTitle("Wallet");

    // Add columns to model view model
    _walletTableViewModel.setHorizontalHeaderItem(0, new QStandardItem("Date"));
    _walletTableViewModel.setHorizontalHeaderItem(0, new QStandardItem("Type"));
    _walletTableViewModel.setHorizontalHeaderItem(0, new QStandardItem("Description"));
    _walletTableViewModel.setHorizontalHeaderItem(0, new QStandardItem("Value"));

    // Set table model to view model
    ui->walletTableView->setModel(&_walletTableViewModel);

    // Get wallet balance
    updateWalletTableView();
}

void WalletDialog::updateWalletTableView() {

}

WalletDialog::~WalletDialog()
{
    delete ui;
}
