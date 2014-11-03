
#ifndef LISTEN_ON_EXCEPTION_H
#define LISTEN_ON_EXCEPTION_H

#include <boost/system/error_code.hpp>

class ListenOnException : public std::exception {

private:
	boost::system::error_code listenOnErrorCode;

public:
	ListenOnException(boost::system::error_code listenOnErrorCode_);
	const char * what() const throw ();
};

#endif