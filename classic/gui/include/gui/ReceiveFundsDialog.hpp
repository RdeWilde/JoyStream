#ifndef RECEIVEFUNDSDIALOG_HPP
#define RECEIVEFUNDSDIALOG_HPP

#include <QDialog>
#include <common/P2PKHAddress.hpp>


namespace Ui {
class ReceiveFundsDialog;
}

class ReceiveFundsDialog : public QDialog
{
    Q_OBJECT

public:

    explicit ReceiveFundsDialog(QWidget * parent);

    ~ReceiveFundsDialog();

    void setReceiveAddress(const Coin::P2PKHAddress &);

signals:

    void freeCoinsPushButtonClicked();

private slots:

    void on_closePushButton_clicked();

private:

    Ui::ReceiveFundsDialog *ui;
};

#endif // RECEIVEFUNDSDIALOG_HPP
