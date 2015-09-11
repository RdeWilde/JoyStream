/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 14 2015
 */

#include <wallet/Utilities.hpp>

#include <QByteArray>
#include <QDateTime>

namespace Wallet {
namespace Utilities {

QByteArray encodeDateTime(const QDateTime & dateTime) {
    return QByteArray::number(dateTime.toMSecsSinceEpoch());
}

QDateTime decodeDateTime(const QByteArray & encodedDateTime) {

    bool ok;
    //long long to handle large values on 32-bit systems
    qulonglong dateTimeMSecsSinceEpoch = encodedDateTime.toULongLong(&ok);

    Q_ASSERT(ok);

    return QDateTime::fromMSecsSinceEpoch(dateTimeMSecsSinceEpoch);
}

}
}
