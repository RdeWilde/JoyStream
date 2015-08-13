/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 9 2015
 */

#ifndef TIME_TOOLS_HPP
#define TIME_TOOLS_HPP

#include <QtGlobal> // quint32

class QTime;

namespace TimeTools {

// Converts x-> #units in x
quint8 hoursInSeconds(quint32 seconds);
quint8 minutesInSeconds(quint32 seconds);
quint8 secondsInSeconds(quint32 seconds);

QTime secondsToQTime(quint32 seconds);

}

#endif // TIME_TOOLS_HPP

