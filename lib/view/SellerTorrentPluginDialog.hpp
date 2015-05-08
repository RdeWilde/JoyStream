#ifndef SELLER_TORRENT_PLUGIN_DIALOG_HPP
#define SELLER_TORRENT_PLUGIN_DIALOG_HPP

#include <QDialog>

#include "extension/SellerTorrentPlugin.hpp"

class SellerTorrentPluginViewModel;

namespace Ui {
class SellerTorrentPluginDialog;
}

class SellerTorrentPluginDialog : public QDialog
{
    Q_OBJECT

public:

    explicit SellerTorrentPluginDialog(QWidget * parent, SellerTorrentPluginViewModel * model);

    ~SellerTorrentPluginDialog();

    // Converts view information into configuration
    SellerTorrentPlugin::Configuration configuration() const;

public slots:

    void updateConfiguration(const SellerTorrentPlugin::Configuration & configuration);

signals:

    // Configuration was updated
    //void configurationUpdated(const SellerTorrentPlugin::Configuration & configuration);

private:
    Ui::SellerTorrentPluginDialog *ui;
};

#endif // SELLER_TORRENT_PLUGIN_DIALOG_HPP
