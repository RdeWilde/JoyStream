/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 28 2016
 */

#ifndef EXTENSION_STATUS_PLUGIN_HPP
#define EXTENSION_STATUS_PLUGIN_HPP

#include <QtGlobal> // temproary, ditch qt integer types asap

namespace joystream {
namespace extension {
namespace status {

    class Plugin {

    public:

        // Default constructor
        Plugin();

        // Constructor from members
        Plugin(quint64 totalReceivedSinceStart, quint64 totalSentSinceStart, quint64 _totalCurrentlyLockedInChannels);

        // Getters and setters
        quint64 totalReceivedSinceStart() const;
        void setTotalReceivedSinceStart(quint64 totalReceivedSinceStart);

        quint64 totalSentSinceStart() const;
        void setTotalSentSinceStart(quint64 totalSentSinceStart);

        quint64 totalCurrentlyLockedInChannels() const;
        void setTotalCurrentlyLockedInChannels(quint64 totalCurrentlyLockedInChannels);

    private:

        // Amount of funds (satoshies) received since start
        quint64 _totalReceivedSinceStart;

        // Amount of funds (satoshies) sent since start
        quint64 _totalSentSinceStart;

        // Amount of funds (satoshies) presently locked
        // in channels started during this session.
        // Obviosuly does not include change in channels!
        quint64 _totalCurrentlyLockedInChannels;
    };

}
}
}

#endif // EXTENSION_STATUS_PLUGIN_HPP
