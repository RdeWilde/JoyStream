/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#include <common/TimeTools.hpp>

#include <cmath>
#include <ctime>
#include <chrono>

namespace TimeTools {

uint8_t hoursInSeconds(uint32_t seconds) {
    return (uint8_t)floor(seconds / 3600);
}

uint8_t minutesInSeconds(uint32_t seconds) {
    return (uint8_t)floor(((seconds - 3600 * hoursInSeconds(seconds)) / 60));
}

uint8_t secondsInSeconds(uint32_t seconds) {
    return uint8_t(seconds - hoursInSeconds(seconds)*3600 - minutesInSeconds(seconds)*60);
}

/*
QTime secondsToQTime(uint32_t seconds) {

    quint8 h = hoursInSeconds(seconds);
    quint8 m = minutesInSeconds(seconds);
    quint8 s = secondsInSeconds(seconds);

    return QTime(h,m,s);
}
*/

}
