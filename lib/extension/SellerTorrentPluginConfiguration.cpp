#include "SellerTorrentPluginConfiguration.hpp"

SellerTorrentPluginConfiguration(const SellerTorrentPluginConfiguration & c)
    : SellerTorrentPluginConfiguration(c.enableBanningSets(), c.minPrice(), c.minLock(), c.minFeePerByte(), c.maxContractConfirmationDelay()) {
}

SellerTorrentPluginConfiguration::SellerTorrentPluginConfiguration(bool enableBanningSets, quint64 minPrice, QTime minLock, quint64 minFeePerByte, QTime maxContractConfirmationDelay)
    : TorrentPluginConfiguration(enableBanningSets)
    , _minPrice(minPrice)
    , _minLock(minLock)
    , _minFeePerByte(minFeePerByte)
    , _maxContractConfirmationDelay(maxContractConfirmationDelay) {
}

SellerTorrentPluginConfiguration::SellerTorrentPluginConfiguration(const libtorrent::entry::dictionary_type & dictionaryEntry)
    : TorrentPluginConfiguration(dictionaryEntry) {
    // IMPLEMENT LATER
}

void SellerTorrentPluginConfiguration::toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const {

    // Call super version
    TorrentPluginConfiguration::toDictionaryEntry(dictionaryEntry);

    // IMPLEMENT LATER
}

quint64 SellerTorrentPluginConfiguration::minPrice() const {
    return _minPrice;
}

void SellerTorrentPluginConfiguration::setMinPrice(const quint64 &minPrice) {
    _minPrice = minPrice;
}
QTime SellerTorrentPluginConfiguration::minLock() const {
    return _minLock;
}

void SellerTorrentPluginConfiguration::setMinLock(const QTime &minLock) {
    _minLock = minLock;
}
quint64 SellerTorrentPluginConfiguration::minFeePerByte() const {
    return _minFeePerByte;
}

void SellerTorrentPluginConfiguration::setMinFeePerByte(const quint64 &minFeePerByte) {
    _minFeePerByte = minFeePerByte;
}
QTime SellerTorrentPluginConfiguration::maxContractConfirmationDelay() const {
    return _maxContractConfirmationDelay;
}

void SellerTorrentPluginConfiguration::setMaxContractConfirmationDelay(const QTime &maxContractConfirmationDelay) {
    _maxContractConfirmationDelay = maxContractConfirmationDelay;
}
