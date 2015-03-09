#include "WalletDialog.hpp"
#include "ui_walletdialog.h"

#include <QInputDialog>
#include <QMessageBox>

#include "extension/BitCoin/Wallet.hpp"

//WalletDialog::WalletDialog(QWidget *parent, Wallet * wallet) {}

WalletDialog::WalletDialog(Wallet * wallet) :
    ui(new Ui::WalletDialog),
    _wallet(wallet) {
    ui->setupUi(this);

    // Setup View model
    this->setWindowTitle("Wallet");

    // Add columns to model view model
    _walletTableViewModel.setHorizontalHeaderItem(0, new QStandardItem("Date"));
    _walletTableViewModel.setHorizontalHeaderItem(1, new QStandardItem("Type"));
    _walletTableViewModel.setHorizontalHeaderItem(2, new QStandardItem("State"));
    _walletTableViewModel.setHorizontalHeaderItem(3, new QStandardItem("Description"));
    _walletTableViewModel.setHorizontalHeaderItem(4, new QStandardItem("Value"));
    _walletTableViewModel.setHorizontalHeaderItem(5, new QStandardItem("#Confirmations"));
    _walletTableViewModel.setHorizontalHeaderItem(6, new QStandardItem("Spent"));

    // Set table model to view model
    ui->walletTableView->setModel(&_walletTableViewModel);

    // Populate view
    refresh();
}

void WalletDialog::refresh() {

    // Clear view model
    clearWalletTableView();

    // Update view model
    updateWalletTableView();

    // Number of keys in wallet
    ui->numEntriesLabel->setText(QString("#Keys in wallet: ") + QString::number(_wallet->numberOfKeysInWallet()));
}

void WalletDialog::clearWalletTableView() {

    int rowCount = _walletTableViewModel.rowCount();

    for(int i = rowCount-1;i >= 0;i--) {

        // Get list
        QList<QStandardItem *> & list = _walletTableViewModel.takeRow(i);

        for(QList<QStandardItem *>::iterator itr = list.begin();
            itr != list.end();itr++)
            delete *itr;
    }
}

void WalletDialog::updateWalletTableView() {

    // Get wallet entries
    const QMap<PublicKey, Wallet::KeyEntry> & entries = _wallet->entries();

    // Iterate entries
    for(QMap<PublicKey, Wallet::KeyEntry>::const_iterator i = entries.constBegin();
        i != entries.constEnd();i++) {

        // Get entry for key
        const Wallet::KeyEntry & entry = i.value();

        // Get outputs in entry
        const QMap<OutPoint, Wallet::KeyEntry::Output> & outputs = entry.outputs();

        for(QMap<OutPoint, Wallet::KeyEntry::Output>::const_iterator i = outputs.constBegin();
            i != outputs.constEnd();i++) {

            // Get output
            const Wallet::KeyEntry::Output & output = i.value();

            // Create new row
            QList<QStandardItem *> items;

            // Date
            items << new QStandardItem(output.added().toString());

            // Type
            QString type;
            switch(output.type()) {

                case Wallet::KeyEntry::Output::Type::ContractChange: type = "Contract change";
                case Wallet::KeyEntry::Output::Type::ContractOutputChange: type = "Contract output change";
                case Wallet::KeyEntry::Output::Type::ContractOutputRefund: type = "Contract output refund";
                case Wallet::KeyEntry::Output::Type::Payment: type = "Contract Payment";
                case Wallet::KeyEntry::Output::Type::Receive: type = "Receive";
            }

            items << new QStandardItem(type);

            // State
            QString state;
            switch(output.state()) {

                case Wallet::KeyEntry::Output::State::Free: state = "Free";
                case Wallet::KeyEntry::Output::State::Locked: state = "Locked";
            }

            items << new QStandardItem(state);

            // Description
            QString description;
            if(output.type() == Wallet::KeyEntry::Output::Type::Receive)
                description = "Payment channel output.";
            else
                description = entry.description();

            items << new QStandardItem(description);

            // Value
            items << new QStandardItem(output.value());

            // Confirmations
            items << new QStandardItem(_wallet->latestBlockHeight() - output.blockHeight());

            // Spent
            items << new QStandardItem(output.spent() ? "True" : "False");

            // Added to view model
            _walletTableViewModel.appendRow(items);
        }
    }

}

WalletDialog::~WalletDialog() {
    delete ui;
}

void WalletDialog::on_receivePushButton_clicked() {

    // Get description
    bool ok;

    QString description = QInputDialog::getText(this,
                                                tr("QInputDialog::getText()"),
                                                tr("Description:"),
                                                QLineEdit::Normal,
                                                "",
                                                &ok);

    // Check that description was provided
    if(!ok || description.isEmpty())
        return;

    // Get wallet entry
    Wallet::KeyEntry entry = _wallet->addReceiveKey(description);

    // Get address
    QString address = _wallet->toAddress(entry.keyPair().pk());

    // Show address in message box
    QMessageBox msgBox;

    msgBox.setText("Added: " + address);
    msgBox.setDetailedText(address);
    msgBox.exec();

    // Update view model
    refresh();
}
