#include "TorrentStatus.hpp"

TorrentStatus::TorrentStatus(const std::vector<char> & resumeData, bool needsTorrentPluginConfigurationAfterTorrentCheckedAlert)
    : _resumeData(resumeData)
    , _needsTorrentPluginConfigurationAfterTorrentCheckedAlert(needsTorrentPluginConfigurationAfterTorrentCheckedAlert) {
}
