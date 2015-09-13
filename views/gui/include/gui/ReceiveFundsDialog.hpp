#ifndef RECEIVEFUNDSDIALOG_HPP
#define RECEIVEFUNDSDIALOG_HPP

#include <QDialog>
#include <common/P2PKHAddress.hpp>

namespace Wallet {
    class Manager;
}

namespace Ui {
class ReceiveFundsDialog;
}

class ReceiveFundsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiveFundsDialog(Wallet::Manager * wallet);

    ~ReceiveFundsDialog();

private slots:

    void on_freeCoinsPushButton_clicked();
    void on_closePushButton_clicked();


private:

    Ui::ReceiveFundsDialog *ui;

    Wallet::Manager * _wallet;

    Coin::P2PKHAddress _receiveAddres;

};

#endif // RECEIVEFUNDSDIALOG_HPP
