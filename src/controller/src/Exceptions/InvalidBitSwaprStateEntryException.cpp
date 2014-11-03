
#include "controller/include/Exceptions/InvalidBitSwaprStateEntryException.hpp"

InvalidBitSwaprStateEntryException::InvalidBitSwaprStateEntryException(const libtorrent::entry::dictionary_type & state_, const char * message_) : state(state_), message(message_) {}

const char * InvalidBitSwaprStateEntryException::what() const throw () {
	return message;
}
