/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#include <common/TimeTools.hpp>

#include <QtMath>
#include <QTime>

namespace TimeTools {

quint8 hoursInSeconds(quint32 seconds) {
    return qFloor(seconds / 3600);
}

quint8 minutesInSeconds(quint32 seconds) {
    return qFloor(((seconds - 3600 * hoursInSeconds(seconds)) / 60));
}

quint8 secondsInSeconds(quint32 seconds) {
    return seconds - hoursInSeconds(seconds)*3600 - minutesInSeconds(seconds)*60;
}

QTime secondsToQTime(quint32 seconds) {

    quint8 h = hoursInSeconds(seconds);
    quint8 m = minutesInSeconds(seconds);
    quint8 s = secondsInSeconds(seconds);

    return QTime(h,m,s);
}

}
