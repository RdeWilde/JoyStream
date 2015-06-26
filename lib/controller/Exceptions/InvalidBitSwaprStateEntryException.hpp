/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */


#ifndef INVALID_BITSWAPR_STATE_ENTRY_EXCEPTION
#define INVALID_BITSWAPR_STATE_ENTRY_EXCEPTION

#include <libtorrent\entry.hpp>

class InvalidBitSwaprStateEntryException : public std::exception {

private:
	const libtorrent::entry::dictionary_type & state;
	const char * message;

public:
	InvalidBitSwaprStateEntryException(const libtorrent::entry::dictionary_type & s, const char *);
	const char * what() const throw ();
};

#endif
