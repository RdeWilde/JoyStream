#include "TorrentWithPluginConfiguration.hpp"

TorrentWithPluginConfiguration::TorrentWithPluginConfiguration(const libtorrent::sha1_hash & info_hash, const std::string & name, const std::string & save_path, const std::vector<char> & resume_data, quint64 flags, qint32 maxPrice, qint32 maxBurn, bool enableBanningSets)
    : TorrentConfiguration(info_hash, name, save_path, resume_data, flags)
    , _maxPrice(maxPrice)
    , _maxBurn(maxBurn)
    , _enableBanningSets(enableBanningSets) {


    damn!! how do we set these in the sub class? call some sort of set routine?

}

