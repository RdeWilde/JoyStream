/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

#ifndef JOYSTREAM_APPKIT_SETTINGS_HPP
#define JOYSTREAM_APPKIT_SETTINGS_HPP

#include <QString>
#include <QJsonValue>

#include <common/Network.hpp>

namespace joystream {
namespace appkit {

struct Settings
{
    static Settings fromJson(const QJsonValue &);
    QJsonValue toJson() const;

    std::string bitcoinNodeHost = "";
    int bitcoinNodePort = 0;

    bool autoStartWalletSync = true;
};

} // appkit
} // joystream

#endif // JOYSTREAM_APPKIT_SETTINGS_HPP
