#ifndef SELLER_TORRENT_PLUGIN_DIALOG_HPP
#define SELLER_TORRENT_PLUGIN_DIALOG_HPP

#include <QDialog>
#include <QStandardItemModel>

//#include "extension/SellerTorrentPlugin.hpp"

class SellerTorrentPluginViewModel;

namespace Ui {
class SellerTorrentPluginDialog;
}

class SellerTorrentPluginDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    SellerTorrentPluginDialog(const SellerTorrentPluginViewModel * model);

    // Destructor
    ~SellerTorrentPluginDialog();

    // Converts view information into configuration
    //SellerTorrentPlugin::Configuration configuration() const;

signals:


private:

    Ui::SellerTorrentPluginDialog *ui;

    // Seller peer plugin table view model
    QStandardItemModel _sellerPeerPluginTableViewModel;
};

#endif // SELLER_TORRENT_PLUGIN_DIALOG_HPP
