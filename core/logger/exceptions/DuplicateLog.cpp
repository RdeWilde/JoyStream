/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "DuplicateLog.hpp"

DuplicateLog::DuplicateLog(const QString & name)
    : name_(name)
{
    whatMessage += name_.toStdString() + " is already registered with category logger.";
}

const char * DuplicateLog::what() const throw() {
    return whatMessage.c_str();
}
