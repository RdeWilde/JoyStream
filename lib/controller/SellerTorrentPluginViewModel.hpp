#ifndef SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP
#define SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP

//#include "TorrentPluginViewModel.hpp"
#include "extension/SellerTorrentPlugin.hpp"

#include <libtorrent/socket_io.hpp>

class SellerPeerPluginViewModel;

class SellerTorrentPluginViewModel : public QObject
{
    Q_OBJECT

public:

    // Constructor
    SellerTorrentPluginViewModel(const SellerTorrentPlugin::Status & status);

    // Destuctor
    ~SellerTorrentPluginViewModel();

    // Add a model view for a new seller peer plugin
    void addPeer(const libtorrent::tcp::endpoint & endPoint, const SellerPeerPlugin::Status & status);

    // Update
    void update(const SellerTorrentPlugin::Status & status);

    // Getters
    quint64 minPrice() const;
    quint32 minLock() const;
    quint64 minFeePerByte() const;
    quint32 maxNumberOfSellers() const;
    quint32 maxContractConfirmationDelay() const;
    QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *> sellerPeerPluginViewModels() const;

signals:

    // Status changed
    void minPriceChanged(quint64 minPrice);
    void minLockChanged(quint32 minLock);
    void minFeePerByteChanged(quint64 minFeePerByte);
    void maxNumberOfSellersChanged(quint32 maxNumberOfSellers);
    void maxContractConfirmationDelayChanged(quint32 maxContractConfirmationDelay);

private:

    // Maximum price accepted (satoshies)
    quint64 _minPrice;

    // Minimum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
    quint32 _minLock;

    // Minimum fee per byte in contract transaction (satoshies)
    quint64 _minFeePerByte;

    // Number of sellers
    quint32 _maxNumberOfSellers;

    // Maximum time (s) for which seller is willing to seed without contract getting at least one confirmation
    quint32 _maxContractConfirmationDelay;

    // Maps endpoint to view model for corresponding seller peer plugin
    // We use pointers since SellerPeerPluginViewModel are QObjects
    QMap<libtorrent::tcp::endpoint, SellerPeerPluginViewModel *> _sellerPeerPluginViewModels;
};

#endif // SELLER_TORRENT_PLUGIN_VIEW_MODEL_HPP
