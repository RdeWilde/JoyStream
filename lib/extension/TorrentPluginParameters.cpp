#include "TorrentPluginParameters.hpp"

TorrentPluginParameters::TorrentPluginParameters(qint32 maxPrice, qint32 maxBurn, bool enableBanningSets)
    : maxPrice_(maxPrice)
    , maxBurn_(maxBurn)
    , enableBanningSets_(enableBanningSets)
{}
