/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/controller/exceptions/ListenOnException.hpp>

ListenOnException::ListenOnException(boost::system::error_code listenOnErrorCode_) : listenOnErrorCode(listenOnErrorCode_) { }

const char * ListenOnException::what() const throw () {
	return listenOnErrorCode.message().c_str();
}
