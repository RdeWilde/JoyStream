
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