/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include "CannnotOpenLogFile.hpp"

CannnotOpenLogFile::CannnotOpenLogFile(const QFile * file) : file_(file) {
    whatMessage += file_->fileName().toStdString() + " is already registered with category logger.";
}

const char * CannnotOpenLogFile::what() const throw() {
    return whatMessage.c_str();
}
