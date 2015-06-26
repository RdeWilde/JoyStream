/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */


#include "InvalidBitSwaprStateEntryException.hpp"

InvalidBitSwaprStateEntryException::InvalidBitSwaprStateEntryException(const libtorrent::entry::dictionary_type & state_, const char * message_) : state(state_), message(message_) {}

const char * InvalidBitSwaprStateEntryException::what() const throw () {
	return message;
}
