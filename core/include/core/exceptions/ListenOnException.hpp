/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */


#ifndef LISTEN_ON_EXCEPTION_HPP
#define LISTEN_ON_EXCEPTION_HPP

#ifndef Q_MOC_RUN
#include <boost/system/error_code.hpp>
#endif Q_MOC_RUN

class ListenOnException : public std::exception {

public:
        ListenOnException(boost::system::error_code listenOnErrorCode_);
        const char * what() const throw ();

private:

        boost::system::error_code listenOnErrorCode;
};

#endif LISTEN_ON_EXCEPTION_HPP
