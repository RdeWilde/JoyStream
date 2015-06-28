/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */


#ifndef LISTEN_ON_EXCEPTION_H
#define LISTEN_ON_EXCEPTION_H

#ifndef Q_MOC_RUN
#include <boost/system/error_code.hpp>
#endif Q_MOC_RUN

class ListenOnException : public std::exception {

private:
	boost::system::error_code listenOnErrorCode;

public:
	ListenOnException(boost::system::error_code listenOnErrorCode_);
	const char * what() const throw ();
};

#endif
