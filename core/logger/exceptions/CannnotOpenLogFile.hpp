/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef CANNNOT_OPEN_LOG_FILE_HPP
#define CANNNOT_OPEN_LOG_FILE_HPP

#include <exception>
#include <QFile>

class CannnotOpenLogFile : public std::exception
{
private:

    // Message returned in what()
    std::string whatMessage;

public:

    // Pointer to file which could not be opened
    const QFile * file_;

    // Constructor
    CannnotOpenLogFile(const QFile * file);

    // Message
    virtual const char * what() const throw();
};

#endif // CANNNOT_OPEN_LOG_FILE_HPP
