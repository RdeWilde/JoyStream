#include "BuyerTorrentPluginConfiguration.hpp"

BuyerTorrentPluginConfiguration::BuyerTorrentPluginConfiguration(const BuyerTorrentPluginConfiguration & c)
    : BuyerTorrentPluginConfiguration(c.enableBanningSets(), c.maxPrice(), c.maxLock(), c.maxFeePerByte(), c.numSellers()) {
}

BuyerTorrentPluginConfiguration::BuyerTorrentPluginConfiguration(bool enableBanningSets, quint64 maxPrice, QTime maxLock, quint64 maxFeePerByte, qint32 numSellers)
    : TorrentPluginConfiguration(enableBanningSets)
    , _maxPrice(maxPrice)
    , _maxLock(maxLock)
    , _maxFeePerByte(maxFeePerByte)
    , _numSellers(numSellers) {
}

BuyerTorrentPluginConfiguration::BuyerTorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry)
    : TorrentPluginConfiguration(dictionaryEntry) {
    // IMPLEMENT LATER
}

void BuyerTorrentPluginConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    // Call super version
    TorrentPluginConfiguration::toDictionaryEntry(dictionaryEntry);

    // IMPLEMENT LATER
}

quint64 BuyerTorrentPluginConfiguration::maxPrice() const {
    return _maxPrice;
}

void BuyerTorrentPluginConfiguration::setMaxPrice(const quint64 &maxPrice) {
    _maxPrice = maxPrice;
}

QTime BuyerTorrentPluginConfiguration::maxLock() const {
    return _maxLock;
}

void BuyerTorrentPluginConfiguration::setMaxLock(const QTime &maxLock) {
    _maxLock = maxLock;
}

quint64 BuyerTorrentPluginConfiguration::maxFeePerByte() const {
    return _maxFeePerByte;
}

void BuyerTorrentPluginConfiguration::setMaxFeePerByte(const quint64 &maxFeePerByte) {
    _maxFeePerByte = maxFeePerByte;
}

qint32 BuyerTorrentPluginConfiguration::numSellers() const {
    return _numSellers;
}

void BuyerTorrentPluginConfiguration::setNumSellers(const qint32 &numSellers) {
    _numSellers = numSellers;
}
