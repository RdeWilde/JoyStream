#ifndef BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP
#define BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP

#include <QDialog>

#include <libtorrent/peer_id.hpp> // sha1_hash
#include <libtorrent/torrent_info.hpp>

namespace Ui {
class BuyerTorrentPluginConfigurationDialog;
}

class Controller;
class Wallet;

class BuyerTorrentPluginConfigurationDialog : public QDialog
{
    Q_OBJECT

public:

    //explicit BuyerTorrentPluginConfigurationDialog(QWidget *parent = 0);

    BuyerTorrentPluginConfigurationDialog(Controller * controller, Wallet * wallet, const libtorrent::torrent_info & torrentInfo);

    // Destructor
    ~BuyerTorrentPluginConfigurationDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_maxTotalSpendLineEdit_textChanged(const QString &arg1);

    void on_feePrKbLineEdit_textChanged(const QString &arg1);

    void on_numPeersLineEdit_textEdited(const QString &arg1);

private:
    Ui::BuyerTorrentPluginConfigurationDialog *ui;

    Controller * _controller;

    Wallet * _wallet;

    libtorrent::torrent_info _torrentInfo;

    //
    quint64 maxPriceFromTotalSpend(quint64 maxTotalSpend, quint64 totalFee);

    //
    quint64 minimalFunds(quint64 maxPrice, qint32 numberOfSellers, quint64 totalFee);

    // Updates the total expenditure field based on the current configuration values
    void updateTotal();
};

#endif // BUYERTORRENTPLUGINCONFIGURATIONDIALOG_HPP
