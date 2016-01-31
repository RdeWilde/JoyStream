/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#include <extension/status/Plugin.hpp>

namespace joystream {
namespace extension {
namespace status {

    Plugin::Plugin()
        : _totalReceivedSinceStart(0)
        , _totalSentSinceStart(0)
        , _totalCurrentlyLockedInChannels(0) {
    }

    Plugin::Plugin(quint64 totalReceivedSinceStart, quint64 totalSentSinceStart, quint64 totalCurrentlyLockedInChannels)
        : _totalReceivedSinceStart(totalReceivedSinceStart)
        , _totalSentSinceStart(totalSentSinceStart)
        , _totalCurrentlyLockedInChannels(totalCurrentlyLockedInChannels){
    }

    quint64 Plugin::totalCurrentlyLockedInChannels() const {
        return _totalCurrentlyLockedInChannels;
    }

    void Plugin::setTotalCurrentlyLockedInChannels(quint64 totalCurrentlyLockedInChannels) {
        _totalCurrentlyLockedInChannels = totalCurrentlyLockedInChannels;
    }

    quint64 Plugin::totalSentSinceStart() const {
        return _totalSentSinceStart;
    }

    void Plugin::setTotalSentSinceStart(quint64 totalSentSinceStart) {
        _totalSentSinceStart = totalSentSinceStart;
    }

    quint64 Plugin::totalReceivedSinceStart() const
    {
        return _totalReceivedSinceStart;
    }

    void Plugin::setTotalReceivedSinceStart(quint64 totalReceivedSinceStart) {
        _totalReceivedSinceStart = totalReceivedSinceStart;
    }


}
}
}
