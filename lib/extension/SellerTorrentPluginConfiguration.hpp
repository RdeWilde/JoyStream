#ifndef SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP
#define SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP

#include "TorrentPluginConfiguration.hpp"

#include <QTime>

class SellerTorrentPluginConfiguration : public TorrentPluginConfiguration {

public:

    // Construct from copy
    SellerTorrentPluginConfiguration(const SellerTorrentPluginConfiguration & c);

    // Constructor from members
    SellerTorrentPluginConfiguration(bool enableBanningSets, quint64 minPrice, QTime minLock, quint64 minFeePerByte, QTime maxContractConfirmationDelay);

    // Constructor from dictionary
    SellerTorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    /**
     * Write configuration into dictionary
     * ===============================================================
     *
     * Seller torrent plugin configuration as it persists across sessions on disk
     * encoded as entry::dictionary_type with the following keys:
     *
     * IMPLEMENT LATER
     *
     */
    void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

    // Getters and setters
    quint64 minPrice() const;
    void setMinPrice(const quint64 &minPrice);

    QTime minLock() const;
    void setMinLock(const QTime &minLock);

    quint64 minFeePerByte() const;
    void setMinFeePerByte(const quint64 &minFeePerByte);

    QTime maxContractConfirmationDelay() const;
    void setMaxContractConfirmationDelay(const QTime &maxContractConfirmationDelay);

private:

    // Maximum price accepted (satoshies)
    quint64 _minPrice;

    // Minimum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
    QTime _minLock;

    // Minimum fee per byte in contract transaction (satoshies)
    quint64 _minFeePerByte;

    // Maximum time (s) for which seller is willing to seed without contract getting at least one confirmation
    QTime _maxContractConfirmationDelay;
};

#endif // SELLER_TORRENT_PLUGIN_CONFIGURATION_HPP
