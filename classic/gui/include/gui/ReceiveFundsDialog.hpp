#ifndef RECEIVEFUNDSDIALOG_HPP
#define RECEIVEFUNDSDIALOG_HPP

#include <QDialog>
#include <common/P2PKHAddress.hpp>

namespace joystream {
namespace bitcoin {
    class SPVWallet;
}
}

namespace Ui {
class ReceiveFundsDialog;
}

class ReceiveFundsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReceiveFundsDialog(joystream::bitcoin::SPVWallet * wallet);

    ~ReceiveFundsDialog();

private slots:

    void on_freeCoinsPushButton_clicked();
    void on_closePushButton_clicked();


private:

    Ui::ReceiveFundsDialog *ui;

    joystream::bitcoin::SPVWallet * _wallet;

    Coin::P2PKHAddress _receiveAddres;

};

#endif // RECEIVEFUNDSDIALOG_HPP
