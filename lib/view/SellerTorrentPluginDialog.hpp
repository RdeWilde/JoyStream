#ifndef SELLER_TORRENT_PLUGIN_DIALOG_HPP
#define SELLER_TORRENT_PLUGIN_DIALOG_HPP

#include "extension/SellerTorrentPlugin.hpp"

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class SellerTorrentPluginDialog;
}

class SellerTorrentPluginViewModel;
class SellerPeerPluginViewModel;
class SellerPeerPluginView;

class SellerTorrentPluginDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    SellerTorrentPluginDialog(QWidget * parent,
                              const SellerTorrentPluginViewModel * model);

    // Destructor
    ~SellerTorrentPluginDialog();

    // Text conversion routines
    static QString minPriceToString(quint64 minPrice);
    static QString minLockTimeToString(quint32 minLockTime);
    static QString minFeePerByteToString(quint64 minFeePerByte);

    // Converts view information into configuration
    //SellerTorrentPlugin::Configuration configuration() const;

public slots:

    // Adds peer
    void addPeer(const SellerPeerPluginViewModel * model);

    // Remove peer
    void removePeer(const libtorrent::tcp::endpoint & endPoint);

    // Update present configuration??
    // May be relevant, e.g. if another view changes it,
    // but we would need to add plugin Alert for this.
    // Can also be used to just set values upon start.
    void updateMinPrice(quint64 minPrice);
    void updateMinLockTime(quint32 minLockTime);
    void updateMinFeePerByte(quint64 minFeePerByte);
    void updateMaxNumberOfSellers(quint32 maxNumberOfSellers);
    void updateMaxContractConfirmationDelay(quint32 delay);

private:

    Ui::SellerTorrentPluginDialog *ui;

    // Seller peer plugin table view model
    QStandardItemModel _sellerPeerPluginTableViewModel;

    // Views for seller peer plugins
    // Pointers are owned by *this object
    QMap<libtorrent::tcp::endpoint, SellerPeerPluginView *> _sellerPeerPluginViews;

};

#endif // SELLER_TORRENT_PLUGIN_DIALOG_HPP
