/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#include <extension/PluginStatus.hpp>

namespace joystream {
namespace extension {

    PluginStatus::PluginStatus()
        : _totalReceivedSinceStart(0)
        , _totalSentSinceStart(0)
        , _totalCurrentlyLockedInChannels(0) {
    }

    PluginStatus::PluginStatus(quint64 totalReceivedSinceStart, quint64 totalSentSinceStart, quint64 totalCurrentlyLockedInChannels)
        : _totalReceivedSinceStart(totalReceivedSinceStart)
        , _totalSentSinceStart(totalSentSinceStart)
        , _totalCurrentlyLockedInChannels(totalCurrentlyLockedInChannels){
    }

    quint64 PluginStatus::totalCurrentlyLockedInChannels() const {
        return _totalCurrentlyLockedInChannels;
    }

    void PluginStatus::setTotalCurrentlyLockedInChannels(quint64 totalCurrentlyLockedInChannels) {
        _totalCurrentlyLockedInChannels = totalCurrentlyLockedInChannels;
    }

    quint64 PluginStatus::totalSentSinceStart() const {
        return _totalSentSinceStart;
    }

    void PluginStatus::setTotalSentSinceStart(quint64 totalSentSinceStart) {
        _totalSentSinceStart = totalSentSinceStart;
    }

    quint64 PluginStatus::totalReceivedSinceStart() const {
        return _totalReceivedSinceStart;
    }

    void PluginStatus::setTotalReceivedSinceStart(quint64 totalReceivedSinceStart) {
        _totalReceivedSinceStart = totalReceivedSinceStart;
    }


}
}
