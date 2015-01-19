#ifndef BUYER_TORRENT_PLUGIN_CONFIGURATION_HPP
#define BUYER_TORRENT_PLUGIN_CONFIGURATION_HPP

#include "TorrentPluginConfiguration.hpp"

#include <QTime>

class BuyerTorrentPluginConfiguration : public TorrentPluginConfiguration {

public:

    // Constructor from copy
    BuyerTorrentPluginConfiguration(const BuyerTorrentPluginConfiguration & c);

    // Constructor from members
    BuyerTorrentPluginConfiguration(bool enableBanningSets, quint64 maxPrice, QTime maxLock, quint64 maxFeePerByte, qint32 numSellers);

    // Constructor from dictionary
    BuyerTorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry);

    /**
     * Write configuration into dictionary
     * ===============================================================
     *
     * Buyer torrent plugin configuration as it persists across sessions on disk
     * encoded as entry::dictionary_type with the following keys:
     *
     * IMPLEMENT LATER
     *
     */
    void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

    // Getters and setters
    quint64 maxPrice() const;
    void setMaxPrice(const quint64 &maxPrice);

    QTime maxLock() const;
    void setMaxLock(const QTime &maxLock);

    quint64 maxFeePerByte() const;
    void setMaxFeePerByte(const quint64 &maxFeePerByte);

    qint32 numSellers() const;
    void setNumSellers(const qint32 &numSellers);

private:

    // Maximum price accepted (satoshies)
    quint64 _maxPrice;

    // Maximum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
    QTime _maxLock;

    // Maximum fee per byte in contract transaction (satoshies)
    quint64 _maxFeePerByte;

    // Number of seller in payment channel
    qint32 _numSellers;
};

#endif // BUYER_TORRENT_PLUGIN_CONFIGURATION_HPP
